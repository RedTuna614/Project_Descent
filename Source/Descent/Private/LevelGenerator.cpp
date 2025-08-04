// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include "DescentGameBase.h"
#include "PlayerBase.h"
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
	roomBias = 7;
	numExits = 0;
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

	CreateSpawnRoom();
}

void ALevelGenerator::ValidateLevel(bool sizeChange)
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
		else if (Room->neighbors[0] == nullptr && Room->room != Start)
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
	currentLevelSize = roomsSpawned.Num();
	//Complete's the ARoomBase gen cycle and starts the level popluation cycle
	if (currentLevelSize >= levelGenSize)
	{
		GEngine->AddOnScreenDebugMessage(15, 10, FColor::Cyan, "Success", false);
		SpawnDeadEnds();
		return;
	}
	/*
	* Adds rooms that have space for neighbors
	* to the array if no new rooms were spawned this gen cycle
	*/
	if (!sizeChange)
	{
		//GEngine->AddOnScreenDebugMessage(15, 10, FColor::Cyan, "Loop", false);
		for (ARoomBase* Room : roomsSpawned)
		{
			if (Room->CanSpawnNeighbor(world))
			{
				//toSpawn.Add(Room);
				toSpawn.AddUnique(Room);
			}
		}
	}
	//Should only be true if no new rooms can be spawned
	if (toSpawn.IsEmpty())
	{
		//Temporary fix that sends the player back to HUB world
		GEngine->AddOnScreenDebugMessage(15, 10, FColor::Cyan, "Fail", false);
		gameManager->ResetLevel();
	}

	GenLevel();
}

void ALevelGenerator::CreateSpawnRoom()
{
	//Starting room should always be 0 in chamberClasses
	currentLevelSize = 0;
	ARoomBase* spawnRoom = world->SpawnActor<ARoomBase>(chamberClasses[0], { 0,0,0 }, { 0,0,0 }, spawnParams);
	roomsSpawned.Add(spawnRoom);
	toSpawn.Add(spawnRoom);
	currentLevelSize++;
	spawnRoom->playerInside = true;
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
	currentLevelSize--;
	toSpawn.Remove(room);
	roomsSpawned.Remove(room);
	room->DestroyRoom();
}

void ALevelGenerator::GenLevel()
{
	ARoomBase* newRoom;
	TArray<ARoomBase*> needNeighbors;
	int spawnIndex; //Index of the Chamber, Hall, or Stair being spawned
	bool didSpawn = false; //Checks if a room was spawned during the gen cycle
	bool didReplace = false; //Checks if ARoomBase was replaced with Stairs
	bool isGoalAttempt = false; //Checks if the generator is trying to spawn the goalRoom
	int index = 0; //Index of currRoom's doorTransforms

	while (currentLevelSize < levelGenSize)
	{
		didSpawn = false;
		for (ARoomBase* currRoom : toSpawn)
		{
			//currRoom is the room being given neighbors
			didReplace = false;
			index = 0;

			if (currRoom->room == Start || (currRoom->neighbors[0] != nullptr && IsValid(currRoom)))
			{
				for (const FTransform& transform : currRoom->doorTransforms)
				{
					index++;
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
						didSpawn = true;
						break;
					case(1):
						/*
						* If newRoom overlaps it should try to spawn Stair instead to change the Z level
						* that rooms can spawn at, hopefully resulting in less overlaps
						*/
						if (currRoom->room == Hall && currRoom->subRoom == ShortHall)
						{
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
							}
						}
						break;
					case(0):
						//The newRoom is completly Invalid and was Destroyed
						RemoveRoom(newRoom);
						break;
					}
					if (didReplace)
					{
						for (int i = 1; i < currRoom->neighbors.Num(); i++)
						{
							if(currRoom->neighbors[i] != nullptr)
								RemoveRoom(currRoom->neighbors[i]);
						}
						RemoveRoom(currRoom);
						break;
					}
				}

				if (!didReplace)
				{
					toSpawn.Remove(currRoom);
				}
			}
			else
			{
				if (currRoom->neighbors[0] == nullptr)
				{
					currRoom->DestroyIsland();
				}
				else
				{
					currRoom->neighbors[0]->RemoveNeighbor(currRoom);
					RemoveRoom(currRoom);
				}
			}
		}
		toSpawn = needNeighbors;
		needNeighbors.Empty();
		currentLevelSize = roomsSpawned.Num();

		if (!didSpawn && toSpawn.IsEmpty())
		{
			ValidateLevel(false);
			return;
		}
	}

	for (ARoomBase* Room : needNeighbors)
	{
		toSpawn.Add(Room);
	}

	ValidateLevel(true);
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
	ADescentGameBase* gameMode = Cast<ADescentGameBase>(UGameplayStatics::GetGameMode(world));
	//The spawn room should alsways be the first index in roomsSpawned
	roomsSpawned[0]->playerInside = true;
	/*
	* Spawns deadEnd if the ARoomBase neighbor is null
	* Spawns door if the roomType is Chamber and neighbor is not null
	*/
	for (ARoomBase* Room : roomsSpawned)
	{
		if (Room->room == Chamber)
		{
			world->SpawnActor<AActor>(door, Room->GetTransform(), spawnParams);
			//Define Room Content (KillRoom, Treasure, )
			if (numExits < 3 && FMath::RandBool() && Room->subRoom == Exit)
			{
				Room->hasExit = true;
				gameMode->ExitRooms.Add(Room);
				numExits++;
			}
			else
			{
				if (FMath::RandRange(0, 10) < roomBias)
				{
					Room->hasEnemies = true;
					gameMode->numKillRooms++;
					roomBias--;
				}
				else
				{
					Room->hasTreasure = true;
					gameMode->numTreasureRooms++;
					roomBias++;
				}
			}
		}
			
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

	gameMode->roomsSpawned = roomsSpawned;

	if(gameManager->objective == Kill || gameManager->objective == Find)
		gameMode->SetGoal(gameManager->objective);

	//Allows the SpawnRoom to function as an exit room if none spawned
	if (numExits == 0)
	{
		roomsSpawned[0]->hasExit = true;
		roomsSpawned[0]->populate();
	}
}