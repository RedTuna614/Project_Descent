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

USTRUCT(Blueprintable, BlueprintType)
struct FDungeonMod
{
	GENERATED_BODY()

public:

	TArray<int>inactiveMods;
	int activeMods = 0;
	//Charger Mobs now explode on death
	//bool explosiveChargers = false;
	
	//Mobs now have shields (Excluding Sniper decoys)
	bool shieldMobs = false;
	//All enemy projectiles travel faster
	bool fastBullets = false;
	//Changes enemy spawn randomness to favor grunts
	bool moreGrunts = false;
	//Exploding enemies leave behind fire that burns the player
	bool napalmExplosions = false;
	//Grunts move faster, shoot faster, have less health, charge the player at low health, and explode on death
	bool unstableGrunts = false;
	//Sniper decoys now shoot at, and damage the player (Should never have shields)
	bool cloningSnipers = false;
	//Chargers now teleport closer to the player if they're too far away
	bool teleportingChargers = false;
	//Brute enemies move faster and deal more damage the lower their health gets
	bool ragingBrutes = false;
	//Hallways can now spawn with traps (traps should only maim the player not kill)
	bool trappedHalls = false;
	//Crafter enemy can now spawn in rooms
	bool spawnCrafter = false;
	//Rocket enemy can now spawn in rooms
	bool spawnRocket = false;
	//Engineer enemy can now spawn in rooms
	bool spawnEngineer = false;
	//Lootchest's now contain only one weapon
	bool lootStarved = false;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
		TArray<bool>dungeonMods;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
		int levelSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
		int baseLevelSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelGen)
		int difficulty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
		bool useInputSize;

	TArray<int>inactiveMods;
	int activeMods;

	void ActivateMod();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Game)
		void ResetGame();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Game)
		void ResetLevel();
	UFUNCTION(BlueprintCallable, Category = Game)
		void ResetVars();
	UFUNCTION(BlueprintCallable, Category = Game)
		void UpdateScore(int score, bool completeLevel);
};
