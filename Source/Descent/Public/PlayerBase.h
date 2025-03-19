// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "WeaponBase.h"
#include "PlayerHud.h"
#include "PlayerBase.generated.h"

/**
 * 
 */
UENUM(BlueprintType) //Different Player ingame Classes: Detetermines stats, starting weapons, and abilties
enum PlayerClasses : int
{
	Scout UMETA(DisplayName = "Scout"),
	Engineer UMETA(DisplayName = "Engineer"),
	Arcist UMETA(DisplayName = "Arcist")
};

UENUM(BlueprintType)
enum PlayerStates : int
{
	Idle UMETA(DisplayName = "Idle"),
	Running UMETA(DisplayName = "Running"),
	Sprinting UMETA(DisplayName = "Sprinting"),
	Damaged UMETA(DisplayName = "Damaged"),
	Death UMETA(DisplayName = "Death")
};

UCLASS()
class DESCENT_API APlayerBase : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	APlayerBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		TEnumAsByte<PlayerClasses> playerType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
		TEnumAsByte<PlayerStates> state;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) //Currently Equipped Weapons
		TArray<UWeaponBase*> EquippedWeapons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) //Index of the weapon being held 
		int activeWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		bool inCombat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
		bool isInteracting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UPlayerHud* HUD;

	UFUNCTION(BlueprintCallable, Category = State)
		void UpdateState(PlayerStates newState);
	UFUNCTION(BlueprintCallable, Category = Stats)
		void DamagePlayer(float damage);

protected:
	UFUNCTION(BlueprintCallable, Category = Stats)
		void SetStats(PlayerClasses newPlayerType);
private:
	
};
