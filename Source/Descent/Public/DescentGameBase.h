// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameManager.h"
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
	int numTreasureRooms;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
	int goalNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
	bool didWin;

	UFUNCTION(BlueprintCallable, Category = Game)
	void ProgressGoal();

	void SetGoal(MissionType mission);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
