// Fill out your copyright notice in the Description page of Project Settings.

//Master

#include "RoomBase.h"
#include "LevelGenerator.h"
#include "Loot_Chest.h"
#include "Engine/OverlapResult.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/StaticMeshActor.h"
#include "Interactible_Base.h"

// Sets default values
ARoomBase::ARoomBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	isSpawning = true;
	beingDestroyed = false;
	didPopulate = false;
	hasEnemies = false;
	hasTreasure = false;
	playerInside = false;
}

// Called when the game starts or when spawned
void ARoomBase::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add("Room");

	if (room == Start || room == Stair)
	{
		isSpawning = false;
	}

	//Fills neighbors with nullptr
	neighbors.Init(nullptr, doorTransforms.Num() + 1);
	//This might seem counterintuitive but, it's needed for spawning deadEnds
}

// Called every frame
void ARoomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoomBase::populate_Implementation()
{
	//WIP

	if (!didPopulate)
	{
		didPopulate = true;
		UWorld* world = GetWorld();
		FHitResult hit;
		FCollisionQueryParams params;
		FTransform transform;
		FVector spawnLoc;
		int spawnNum;
		AStaticMeshActor* prop;

		params.AddIgnoredComponent(box);
		spawnParams.Owner = this;

		if (hasExit)
		{
			spawnLoc = { roomCenter.X, roomCenter.Y, roomCenter.Z - 470 };
			world->SpawnActor<AInteractible_Base>(exitRoom, spawnLoc, { 0,0,0 }, spawnParams);
		}

		switch (size)
		{
		case(Small):
			//Spawn Props
			break;
		case(Med):
			//Spawn Enemies
			//Spawn Props
			spawnNum = FMath::RandRange(1, 5);
			for (int i = 0; i < spawnNum; i++)
			{
				spawnLoc = UKismetMathLibrary::RandomPointInBoundingBox(roomCenter, boxExtents);
				world->LineTraceSingleByChannel(hit, spawnLoc, { spawnLoc.X, spawnLoc.Y, spawnLoc.Z - 1500 }, ECC_WorldStatic, params);
				if (hit.GetActor() != nullptr)
				{
					if (hit.GetComponent()->GetName().Contains("Floor"))
					{
						prop = world->SpawnActor<AStaticMeshActor>(spawnLoc, { 0,0,0 }, spawnParams);
						prop->GetStaticMeshComponent()->SetStaticMesh(propMeshes[FMath::RandRange(0, propMeshes.Num() - 1)]);
						prop->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
						prop->GetStaticMeshComponent()->SetSimulatePhysics(true);
					}
				}
			}
			break;
		case(Large):
			//Spawn Props
			//Spawn Enemies
			break;
		}

		return;
	}
}

void ARoomBase::SpawnMobs()
{
	UWorld* world = GetWorld();
	FVector spawnLoc;
	FCollisionQueryParams params;
	FHitResult hit;
	int enemyNum = FMath::RandRange(3, 7);
	int i = 0;

	switch (size)
	{
	case(Small):
		enemyNum = FMath::RoundFromZero(enemyNum / 2.0);
		break;
	case(Med):
		enemyNum *= 1;
		break;
	case(Large):
		enemyNum *= 4;
		break;
	}

	params.AddIgnoredComponent(box);
	spawnParams.Owner = this;

	while (i != enemyNum)
	{
		spawnLoc = UKismetMathLibrary::RandomPointInBoundingBox(roomCenter, boxExtents);
		world->LineTraceSingleByChannel(hit, spawnLoc, { spawnLoc.X, spawnLoc.Y, spawnLoc.Z - 1500 }, ECC_WorldStatic, params);
		if (hit.GetActor() != nullptr)
		{
			if (hit.GetComponent()->GetName().Contains("Floor"))
			{
				world->SpawnActor<AEnemyBase>(enemies[FMath::RandRange(0, enemies.Num() - 1)], spawnLoc, { 0,0,0 }, spawnParams);
				i++;
			}
		}
	}
}

void ARoomBase::SpawnTreasure(AActor* player)
{
	UWorld* world = GetWorld();
	FVector spawnLoc;
	FRotator spawnRot;
	FCollisionQueryParams params;
	FHitResult hit;

	params.AddIgnoredComponent(box);
	spawnParams.Owner = this;

	if (subRoom != ModTransfer)
	{
		//Sets the chest's spawn location to 500 units infront of the player when entering the room
		spawnLoc = (player->GetActorForwardVector() * 500) + player->GetActorLocation();
		//Forces the chest to face the player when spawned
		spawnRot = { 0,0,player->GetActorRotation().Yaw * -1 };
	}
	else
	{
		//Sets the treasure's spawn location to the roomCenter
		spawnLoc = roomCenter;
		//Sets the treasure's spawn rotation to the Room's rotation
		spawnRot = GetActorRotation();
	}

	world->SpawnActor<AActor>(treasure, spawnLoc, spawnRot, spawnParams);
}

void ARoomBase::RemoveNeighbor(ARoomBase* neighbor)
{
	for (int i = 0; i < neighbors.Num(); i++)
	{
		if (neighbors[i] == neighbor)
		{
			neighbors[i] = nullptr;
			break;
		}
	}
}

void ARoomBase::ChangeMapColor(bool inMap)
{
	for (UMaterialInstanceDynamic* mat : roomMats)
	{
		mat->SetScalarParameterValue("Opacity", inMap);
		mat->SetScalarParameterValue("isEmissive", inMap);
		mat->SetScalarParameterValue("isEmpty", playerInside);
		mat->SetScalarParameterValue("isExit", hasExit);
	}
}

bool ARoomBase::IsValidRoom(UWorld* world, ARoomBase* spawner)
{
	AActor* overlapedActor; //Actor overlapped or hit
	FQuat roomQuat = GetActorQuat(); //Quaternion of this room
	TArray<FOverlapResult> outOverlaps; //Array of overlapped actors
	FCollisionShape shape = FCollisionShape::MakeBox(boxExtents); //Shape of collision
	FCollisionQueryParams collisionParams; //Additional params for Overlap and Sweep
	collisionParams.AddIgnoredActor(this); //While testing I'm not sure if this actually works in 5.4
	collisionParams.AddIgnoredActor(spawner);
	/*
	* I'm not sure if AddIgnoredActor works in 5.4, but I don't want to check if a room is overlapping the
	* one that caused it to be spawned, because if it is, it's by an extremely small amount that is ignorable
	*/

	//Checks if the room spawning this one still exists, Start can't be deleted so it can be ignored
	//Due to the change in z level this realy can't be checked with stairs
	if (room != Stair && spawner->room != Stair && spawner->room != Start)
	{
		FHitResult outHit;
		world->LineTraceSingleByChannel(outHit, roomCenter, spawner->roomCenter, ECC_WorldStatic, collisionParams);
		overlapedActor = outHit.GetActor();
		if (IsValid(overlapedActor))
		{
			if (overlapedActor != this && overlapedActor != spawner)
			{
				DestroyRoom();
				return false;
			}
		}
	}

	//Checks if another room overlaps this one
	world->OverlapMultiByChannel(outOverlaps, roomCenter, roomQuat, ECC_WorldStatic, shape, collisionParams);
	if (!outOverlaps.IsEmpty())
	{
		for (FOverlapResult& overlap : outOverlaps)
		{
			overlapedActor = overlap.GetActor();
			if (IsValid(overlapedActor))
			{
				if (overlapedActor->ActorHasTag("Room"))
				{
					//The below if was added, as it seemed the AddIgnoredActor wasn't working while testing
					if (overlapedActor != spawner && overlapedActor != this)
					{
						if (!Cast<ARoomBase>(overlapedActor)->isSpawning && !Cast<ARoomBase>(overlapedActor)->beingDestroyed)
						{
							//GEngine->AddOnScreenDebugMessage(0, 10, FColor::Red, overlapedActor->GetActorNameOrLabel(), false);
							//DrawDebugBox(world, roomCenter, boxExtents, roomQuat, FColor::Red, false, 10);
							DestroyRoom();
							return false;
						}
					}
				}
			}
		}
	}

	TArray<FHitResult>hits;
	world->SweepMultiByChannel(hits, roomCenter, shape.GetBox(), roomQuat, ECC_Visibility, shape);

	if (!hits.IsEmpty())
	{
		for (FHitResult& hit : hits)
		{
			overlapedActor = hit.GetActor();
			if (IsValid(overlapedActor))
			{
				if (overlapedActor->ActorHasTag("Room"))
				{
					if (Cast<ARoomBase>(overlapedActor)->room != Start)
					{
						//The below if was added, as it seemed the AddIgnoredActor wasn't working while testing
						if (overlapedActor != spawner && overlapedActor != this)
						{
							if (!Cast<ARoomBase>(overlapedActor)->isSpawning && !Cast<ARoomBase>(overlapedActor)->beingDestroyed)
							{
								//GEngine->AddOnScreenDebugMessage(0, 10, FColor::Red, overlapedActor->GetActorNameOrLabel(), false);
								//DrawDebugBox(world, roomCenter, boxExtents, roomQuat, FColor::Red, false, 10);
								DestroyRoom();
								return false;
							}
						}
					}
				}
			}
		}
	}

	//DrawDebugBox(world, roomCenter, boxExtents, roomQuat, FColor::Green, false, 10);
	//At this point the room is valid and has finished spawning
	isSpawning = false;
	return true;

}

bool ARoomBase::CanSpawnNeighbor(UWorld* world)
{
	FHitResult outHit;
	FVector dir, endPos, startPos;
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);

	for (int i = 1; i < neighbors.Num(); i++)
	{
		if (neighbors[i] == nullptr)
		{
			dir = UKismetMathLibrary::GetForwardVector(doorTransforms[i - 1].Rotator());
			startPos = doorTransforms[i - 1].GetLocation();
			endPos = ((dir * 500) + startPos);
			world->LineTraceSingleByChannel(outHit, startPos, endPos, ECC_WorldStatic, collisionParams);

			if (outHit.GetActor() == nullptr || !outHit.GetActor()->ActorHasTag("Room"))
			{
				return true;
			}
		}
	}

	return false;
}

void ARoomBase::AddNeighbor(ARoomBase* toAdd, int index)
{
	neighbors[index] = toAdd;
}

void ARoomBase::AddNeighbor(ARoomBase* toAdd)
{
	for (int i = 0; i < neighbors.Num(); i++)
	{
		if (neighbors[i] == nullptr)
		{
			neighbors[i] = toAdd;
			break;
		}
	}
}

void ARoomBase::ReplaceNeighbor(ARoomBase* toReplace, ARoomBase* newRoom)
{
	for (int i = 0; i < neighbors.Num(); i++)
	{
		if (neighbors[i] == toReplace)
		{
			neighbors[i] = newRoom;
			break;
		}
	}
}

void ARoomBase::DestroyRoom()
{
	Destroy();
}

void ARoomBase::DestroyValidator()
{
	//box->DestroyComponent();
	box->SetCollisionResponseToAllChannels(ECR_Ignore);
	box->SetCollisionResponseToChannel(ECC_Camera, ECR_Overlap);
	box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	box->SetCollisionObjectType(ECC_WorldDynamic);
}

void ARoomBase::DestroyIsland()
{
	ALevelGenerator* levelGen = Cast<ALevelGenerator>(Owner);
	for (ARoomBase* neighbor : neighbors)
	{
		if (neighbor != nullptr)
		{
			neighbor->RemoveNeighbor(this);
			neighbor->DestroyIsland();
		}
	}
	levelGen->RemoveRoom(this);
}