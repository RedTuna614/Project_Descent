// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomBase.h"
#include "LevelGenerator.h"
#include "Engine/OverlapResult.h"

// Sets default values
ARoomBase::ARoomBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	spawnBias = 0;
	isSpawning = true;
	beingDestroyed = false;
	didPopulate = false;
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

void ARoomBase::populate()
{
	//WIP

	if (!didPopulate)
	{
		didPopulate = true;

		switch (size)
		{
		case(Small):
			//Spawn Props
			break;
		case(Med):
			//Spawn Props
			//Spawn Enemies
			break;
		case(Large):
			//Spawn Props
			//Spawn Enemies
			break;
		}

		return;
	}
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
	box->SetCollisionResponseToAllChannels(ECR_Overlap);
}