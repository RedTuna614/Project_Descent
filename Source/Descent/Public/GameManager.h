// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameManager.generated.h"

/**
 * 
 */

UENUM(BlueprintType) //Different Room Types
enum MissionType : int
{
	Kill UMETA(DisplayName = "Exterminate"),
	Find UMETA(DisplayName = "Explore")
};

UCLASS()
class DESCENT_API UGameManager : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UGameManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
		TEnumAsByte<MissionType> objective;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
		int levelSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
		bool useInputSize;

	UFUNCTION(BlueprintImplementableEvent, Category = Game)
		void ResetGame();
	UFUNCTION(BlueprintImplementableEvent, Category = Game)
		void ResetLevel();

};
