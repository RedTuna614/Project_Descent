// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomBase.h"
#include "GameManager.h"
#include "LevelGenerator.generated.h"

UCLASS()
class DESCENT_API ALevelGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGenerator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
		TArray<UClass*> chamberClasses; //Array containing the UClasses for Chamber ARoomBase
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
		TArray<UClass*> hallWays; //Array containing the UClasses for Hall ARoomBase
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
		TArray<UClass*> stairWays; //Array containing the UClasses for Stair ARoomBase
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
		TArray<UClass*> goalRooms; //Array containing the UClasses for GoalRoom ARoomBase
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
		TArray<ARoomBase*> roomsSpawned; //Array containing all ARoomBase spawned in the level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
		TEnumAsByte<MissionType> mission; //Enum for determining which goalRoom is spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
		UClass* deadEnd; //UClass for the deadEnd AActor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
		UClass* door; //UClass for the door AActor
		
	UFUNCTION(BlueprintCallable, Category = LevelGen)
		void ValidateLevel(); //Checks and removes ARoomBase that are null or can't be reached
	UFUNCTION(BlueprintCallable, Category = LevelGen)
		void CreateSpawnRoom(); //Spawns the Start ARoomBase in the level and starts the level gen process

	ARoomBase* MakeDeadEnd(ARoomBase* toReplace);
	void SpawnDeadEnds(); //Spawns deadEnds and doors in the level
	void RemoveRoom(ARoomBase* room); //Destroys and removes ARoomBase from the level
	void GenLevel(); //Generates the level
	int ValidateRoom(ARoomBase* room, ARoomBase* spawner); //Returns the valid level of the given ARoomBase
	TArray<ARoomBase*> toSpawn; //Array of ARoomBase that can have neighbors spawned
	int currentLevelSize; //Size the currently generated level (number of ARoomBase spawned)
	int levelGenSize; //Size the level should be generated to

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UGameManager* gameManager; //Game Instance 
	UWorld* world; //The level context
	FActorSpawnParameters spawnParams; //Parameters for all actors spawned by this
	
	int genIteration;
	int roomBias; //Used to control the randomness when choosing room contents
	int numChamberClasses; //Size of chamberClasses array
	int numHallClasses; //Size of hallWays array
	bool spawnedGoalRoom; //Bool checking if the goal room was spawned

};
