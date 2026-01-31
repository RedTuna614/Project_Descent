// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameManager.h"
#include "RoomBase.h"
#include "ArmorMesh_Manager.h"
#include "DescentGameBase.generated.h"

/**
 * 
 */
UCLASS()
class DESCENT_API ADescentGameBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ADescentGameBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
	int numKillRooms;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
	int numChambers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
	int goalNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
	bool didWin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
	TArray<ARoomBase*>ExitRooms;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
	TArray<ARoomBase*>roomsSpawned;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Game)
	AArmorMesh_Manager* armorISM_Manager;

	UFUNCTION(BlueprintCallable, Category = Game)
	void ProgressGoal();
	UFUNCTION(BlueprintCallable, Category = Map)
	void ChangeToMapView(bool inMap);

	void SetGoal(MissionType mission);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
