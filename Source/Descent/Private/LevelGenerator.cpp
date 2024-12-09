// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGenerator.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	currentLevelSize = 0;
	levelGenSize = 0;
	numChamberClasses = 0;
	numHallClasses = 0;
	spawnedGoalRoom = false;
	genIteration = 0;
}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();

	//Gets levelGenSize and mission from gameInstance
	gameManager = GetGameInstance<UGameManager>();
	levelGenSize = gameManager->levelSize;
	mission = gameManager->objective;

	world = GetWorld();
	spawnParams.Owner = this;
	//Random num generation is max min inclusive so the size must equal the last index
	numChamberClasses = chamberClasses.Num() - 1;
	numHallClasses = hallWays.Num() - 1;

	//CreateSpawnRoom();
}

void ALevelGenerator::ValidateLevel()
{
	genIteration++;

	//Array of ARoomBase to be removed from the level
	TArray<ARoomBase*> toRemove;

	//Checks if ARoomBase can be reached and adds it toRemove if it can't
	for (ARoomBase* Room : roomsSpawned)
	{
		if (Room->neighbors.IsEmpty())
		{
			toRemove.Add(Room);
		}
		else if (Room->neighbors[0] == nullptr)
		{
			//neighbors[0] is the ARoomBase that caused Room to spawn
			//If neighbors[0] is null than the Room can't be reached
			toRemove.Add(Room);
			for (ARoomBase* neighbor : Room->neighbors)
			{
				if (neighbor != nullptr)
				{
					neighbor->RemoveNeighbor(Room);
				}
			}
		}
	}

	//Removes invalid rooms from the level and decreases the currentLevelSize
	for (ARoomBase* Room : toRemove)
	{
		currentLevelSize--;
		RemoveRoom(Room);
	}
	
	//Removes Multiple Goal Rooms
	if (spawnedGoalRoom)
	{
		int spawnedGoals = 0;
		for (ARoomBase* Room : roomsSpawned)
		{
			if (Room->room == GoalRoom)
			{
				spawnedGoals++;
				if (spawnedGoals != 1)
				{
					Room->Destroy();
					roomsSpawned.Remove(Room);
				}
			}
		}
	}

	//Forces the level to continue generating if the number of ARoomBase is less than the levelGenSize
	if (roomsSpawned.Num() < levelGenSize && !toSpawn.IsEmpty())
	{
		currentLevelSize = roomsSpawned.Num();
		GenLevel();
	}
	else if (toSpawn.IsEmpty() && roomsSpawned.Num() < levelGenSize)
	{
		//GEngine->AddOnScreenDebugMessage(4, 10, FColor::Cyan, "Empty", false);

		currentLevelSize = roomsSpawned.Num();
		for (ARoomBase* Room : roomsSpawned)
		{
			for (ARoomBase* neighbor : Room->neighbors)
			{
				if (neighbor == nullptr)
				{
					toSpawn.Add(Room);
				}
			}
		}

		GenLevel();
	}
	else //Complete's the ARoomBase gen cycle and starts the level popluation cycle
	{
		//GEngine->AddOnScreenDebugMessage(15, 10, FColor::Cyan, FString::SanitizeFloat(roomsSpawned.Num()), false);
		SpawnDeadEnds();
	}
}

void ALevelGenerator::CreateSpawnRoom()
{
	//Starting room should always be 0 in chamberClasses
	currentLevelSize = 0;
	ARoomBase* spawnRoom = world->SpawnActor<ARoomBase>(chamberClasses[0], { 0,0,0 }, { 0,0,0 }, spawnParams);
	roomsSpawned.Add(spawnRoom);
	toSpawn.Add(spawnRoom);
	currentLevelSize++;
	GenLevel();
}

ARoomBase* ALevelGenerator::MakeDeadEnd(ARoomBase* toReplace)
{
	//MakeDeadEnd before removing
	RemoveRoom(toReplace);
	return nullptr;
}

void ALevelGenerator::RemoveRoom(ARoomBase* room)
{
	for (ARoomBase* isRoom : toSpawn)
	{
		if (isRoom == room)
		{
			toSpawn.Remove(room);
			break;
		}
	}
	for (ARoomBase* isRoom : roomsSpawned)
	{
		if (isRoom == room)
		{
			roomsSpawned.Remove(room);
			break;
		}
	}
	room->DestroyRoom();
}

void ALevelGenerator::GenLevel()
{
	ARoomBase* newRoom;
	TArray<ARoomBase*> needNeighbors;
	int spawnIndex; //Index of the Chamber, Hall, or Stair being spawned
	bool didReplace = false; //Checks if ARoomBase was replaced with Stairs
	int i = 0; //Iteration of the levelGen process where ARoomBase wasn't spawned
	bool isGoalAttempt = false; //Checks if the generator is trying to spawn the goalRoom
	int index = 0; //Index of currRoom's doorTransforms
	int maxGens = 10000; //Total number of allowed iterations(Prevents an infinite loop if nothing can spawn)

	//For Levels larger than size 5000, ensures the loop doesn't end early
	//Mostly here for generator stress testing
	if (levelGenSize >= (maxGens / 2))
		maxGens *= 20;

	while (currentLevelSize < levelGenSize)
	{ 
		i++;
		//Ends the loop early
		if (i == maxGens)
		{
			for (ARoomBase* Room : needNeighbors)
			{
				toSpawn.Add(Room);
			}
			ValidateLevel();
			return;
		}

		for (ARoomBase* currRoom : toSpawn)
		{
			//currRoom is the room being given neighbors
			didReplace = false;
			index = 0;
			if (currentLevelSize >= levelGenSize)
				break;

			if (IsValid(currRoom))
			{
				for (const FTransform& transform : currRoom->doorTransforms)
				{
					index++;
					if (currentLevelSize >= levelGenSize)
						break;

					currentLevelSize++;
					isGoalAttempt = false;

					//Kill mission is the kill a number of mobs, and Find requires exploration so no goalRoom is needed
					if (mission != Kill && mission != Find && !spawnedGoalRoom && (currentLevelSize > levelGenSize * .6) && currRoom->room != Chamber)
					{
						// 0  will be changed later once goalRooms and missionTypes are made
						newRoom = world->SpawnActor<ARoomBase>(goalRooms[0], transform, spawnParams);
						isGoalAttempt = true;
					}
					else
					{
						//A Chamber shouldn't neighbor a Chamber or else doors will overlap
						if (currRoom->room != Hall || FMath::RandRange(0, 1) < 1)
						{
							//Spawns Hall ARoomBase
							spawnIndex = FMath::RandRange(0, numHallClasses);
							newRoom = world->SpawnActor<ARoomBase>(hallWays[spawnIndex], transform, spawnParams);
						}
						else
						{
							//Spawns Chamber ARoomBase
							spawnIndex = FMath::RandRange(1, numChamberClasses);
							newRoom = world->SpawnActor<ARoomBase>(chamberClasses[spawnIndex], transform, spawnParams);
						}
					}
					newRoom->AddNeighbor(currRoom);

					//Checks if newRoom is valid or Overlaps another ARoomBase
					switch (ValidateRoom(newRoom, currRoom))
					{
					case(2): //Completely Valid
						if (isGoalAttempt)
						{
							spawnedGoalRoom = true;
						}
						if (currRoom->neighbors.Num() > 2)
						{
							currRoom->AddNeighbor(newRoom, index);
						}
						else
							currRoom->AddNeighbor(newRoom);
						roomsSpawned.Add(newRoom);
						needNeighbors.Add(newRoom);
						i = 0;
						break;

					case(1):
						/*
						* If newRoom overlaps it should try to spawn Stair instead to change the Z level
						* that rooms can spawn at, hopefully resulting in less overlaps
						*/
						if (currRoom->room == Hall && currRoom->subRoom == ShortHall)
						{
							currentLevelSize--;
							spawnIndex = FMath::RandRange(0, stairWays.Num() - 1);
							newRoom = world->SpawnActor<ARoomBase>(stairWays[spawnIndex], currRoom->GetTransform(), spawnParams);
							//Checks if the Stairs overlap another ARoomBase
							if (newRoom->IsValidRoom(world, currRoom))
							{
								newRoom->AddNeighbor(currRoom->neighbors[0]);
								currRoom->neighbors[0]->ReplaceNeighbor(currRoom, newRoom);
								roomsSpawned.Add(newRoom);
								needNeighbors.Add(newRoom);
								didReplace = true;
								i = 0;
							}
						}
						break;

					case(0):
						//The newRoom is completly Invalid and was Destroyed
						currentLevelSize--;
						break;
					}

					if (didReplace)
					{
						RemoveRoom(currRoom);
						break;
					}
				}

				if (!didReplace)
				{
					toSpawn.Remove(currRoom);
				}
			}
		}
		toSpawn = needNeighbors;
		needNeighbors.Empty();
	}

	for (ARoomBase* Room : needNeighbors)
	{
		toSpawn.Add(Room);
	}

	ValidateLevel();
}

int ALevelGenerator::ValidateRoom(ARoomBase* Room, ARoomBase* spawner)
{
	/*
	* 0 - Room is invalid
	* 1 - Room is valid but overlaps
	* 2 - Room is valid and doesn't overlap
	*/
	int validLevel = 0;
	if (IsValid(Room))
	{
		validLevel = 1;
		if (Room->IsValidRoom(world, spawner))
		{
			validLevel = 2;
		}
	}

	return validLevel;
}

void ALevelGenerator::SpawnDeadEnds()
{	
	/*
	* Spawns deadEnd if the ARoomBase neighbor is null
	* Spawns door if the roomType is Chamber and neighbor is not null
	*/
	for (ARoomBase* Room : roomsSpawned)
	{
		if (Room->room == Chamber)
			world->SpawnActor<AActor>(door, Room->GetTransform(), spawnParams);
		if (Room->room != Start)
		{
			for (int i = 1; i < Room->neighbors.Num(); i++)
			{
				if (Room->neighbors[i] == nullptr)
				{
					world->SpawnActor<AActor>(deadEnd, Room->doorTransforms[i - 1], spawnParams);
				}
				if (Room->neighbors[i] != nullptr && Room->room == Chamber)
				{
					world->SpawnActor<AActor>(door, Room->doorTransforms[i - 1], spawnParams);
				}
			}
		}
		Room->DestroyValidator();
		Room->populate();
	}
}
