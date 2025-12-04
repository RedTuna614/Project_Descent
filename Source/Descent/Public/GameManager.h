// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WeaponBase.h"
#include "WeaponAssembler.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
		TArray<UWeaponBase*> playerWeapons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
		TArray<UWeaponBase*> playerInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		UWeaponAssembler* weaponAssembler;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
		TArray<int>gameScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
		int levelSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
		int baseLevelSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
		int difficulty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
		bool useInputSize;


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Game)
		void ResetGame();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Game)
		void ResetLevel();
	UFUNCTION(BlueprintCallable, Category = Game)
		void ResetVars();
	UFUNCTION(BlueprintCallable, Category = Game)
		void UpdateScore(int score, bool completeLevel);
};
