// Fill out your copyright notice in the Description page of Project Settings.

//Master

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.h"
#include "RoomBase.generated.h"

UENUM(BlueprintType) //Different Room Types
enum RoomType : int
{
	Hall UMETA(DisplayName = "Hall"),
	Stair UMETA(DisplayName = "Stair"),
	Chamber UMETA(DisplayName = "Chamber"),
	Start UMETA(DisplayName = "Start"),
	GoalRoom UMETA(DisplayName = "Goal")
};

UENUM(BlueprintType) //Different Room SubTypes
enum SubRoomType : int
{
	Corner UMETA(DisplayName = "Corner"),
	TriHall UMETA(DisplayName = "TriHall"),
	ShortHall UMETA(DisplayName = "ShortHall"),
	MidHall UMETA(DisplayName = "MidHall"),
	LongHall UMETA(DisplayName = "LongHall"),
	Exit UMETA(DisplayName = "Exit"),
	ModTransfer UMETA(DisplayName = "ModTransfer")
};

UENUM(BlueprintType) //Different Room Sizes
enum RoomSize : int
{
	Small UMETA(DisplayName = "Small"),
	Med UMETA(DisplayName = "Medium"),
	Large UMETA(DisplayName = "Large")
};

UCLASS()
class DESCENT_API ARoomBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoomBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Room)
	TArray<ARoomBase*> neighbors; //ARoomBase connected to this Room
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Room)
	TEnumAsByte<RoomType> room; //The type of room
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Room)
	TEnumAsByte<SubRoomType> subRoom; //The subType of the room
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Room)
	TEnumAsByte<RoomSize> size; //Size of the room
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Room)
	TArray<FTransform> doorTransforms; //This is how the generator knows where to spawn new rooms and doors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Room)
	UPrimitiveComponent* box; //Internal box collider in the room (Helps with overlap testing by making the room not hollow)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Room)
	FVector boxExtents; //Scaled extents of box, and the extents used when checking for room overlap
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Room)
	FVector roomCenter; //Center World Coordinate of the room
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
	TArray<UClass*> enemies; //Array of mob UClasses that can be spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
	UClass* treasure; //UClass for treasure that can be spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
	UClass* exitRoom; //Array of prop UClasses that can be spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
	TArray<UStaticMesh*> propMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Material)
	TArray<UMaterialInstanceDynamic*> roomMats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
	bool beingDestroyed; //Checks if the room is being destroyed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
	bool isSpawning; //Checks if the room is still spawning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Room)
	bool hasEnemies; //Is needed to know if the room should be counted in kill missions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Room)
	bool hasTreasure;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Room)
	bool hasExit; //Need to if the room contains an exit to the next level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Map)
	bool playerInside; 

	bool didPopulate; //Check to see if the room has been populated

	UFUNCTION(BlueprintCallable, Category = LevelGen)
	void RemoveNeighbor(ARoomBase* neighbor); //Removes neighbor from neighbors
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = LevelGen)
	void populate(); //Fills the room with mobs and/or props
	UFUNCTION(BlueprintCallable, Category = Room)
	void SpawnMobs();
	UFUNCTION(BlueprintCallable, Category = Room)
	void SpawnTreasure(AActor* player);
	UFUNCTION(BlueprintCallable, Category = Room)
	void ChangeMapColor(bool inMap); //Changes color of the room the player is in when opening the map

	bool IsValidRoom(UWorld* world, ARoomBase* spawner); //Checks if the room overlaps another room
	bool CanSpawnNeighbor(UWorld* world);
	void AddNeighbor(ARoomBase* toAdd, int index); //Adds toAdd to neighbors at index
	void AddNeighbor(ARoomBase* toAdd); //Replaces the first found nullptr with toAdd in niehgbors
	void ReplaceNeighbor(ARoomBase* toReplace, ARoomBase* newRoom); //Replaces toReplace with newRoom in neighbors
	void DestroyRoom(); //Destroys the room
	void DestroyValidator(); //Changes the collision type of box to overlap
	void DestroyIsland(); //Destroys this room and all neighbors that are discnonnected from the dungeon

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FActorSpawnParameters spawnParams; //Parameters of props/mobs spawned
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
