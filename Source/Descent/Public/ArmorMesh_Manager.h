// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArmorMesh_Manager.generated.h"

USTRUCT(BlueprintType)
struct FMeshId
{
	GENERATED_BODY()

public:
	AStaticMeshActor* mesh;
	int time;
};

UCLASS()
class DESCENT_API AArmorMesh_Manager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArmorMesh_Manager();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TArray<FMeshId> staticMeshes;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool hasInstances;
	UWorld* world;

public:	

	UFUNCTION(BlueprintCallable, Category = ISM)
	void SpawnInstance(UStaticMesh* mesh, FTransform transform);
	UFUNCTION(BlueprintCallable, Category = ISM)
	void ManageInstances();
	UFUNCTION(BlueprintImplementableEvent, Category = Timer)
	void StartTimer();
	UFUNCTION(BlueprintImplementableEvent, Category = Timer)
	void EndTimer();
};
