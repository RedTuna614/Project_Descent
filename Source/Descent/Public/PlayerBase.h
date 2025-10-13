// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "WeaponBase.h"
#include "PlayerHud.h"
#include "RoomBase.h"
#include "InventoryBase.h"
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

//Unreal randomly started complaining that the playerstate and enemystate have enums with the same name
//It's been fine and working with same names way for a year
UENUM(BlueprintType)
enum PlayerStates : int
{
	Idl UMETA(DisplayName = "Idle"),
	Running UMETA(DisplayName = "Running"),
	Sprinting UMETA(DisplayName = "Sprinting"),
	Damaged UMETA(DisplayName = "Damaged"),
	Deth UMETA(DisplayName = "Death")
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game) 
		TArray<UWeaponBase*> Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat) //Stores the player's current shields
		float shields;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat) //Needed to ensure player's shield don't recharge past max
		float maxShields;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) //Index of the weapon being held 
		int activeWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		bool inCombat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
		bool isInteracting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UPlayerHud* HUD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
		ARoomBase* inRoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UInventoryBase* inventoryUI;

	UFUNCTION(BlueprintCallable, Category = State)
		void UpdateState(PlayerStates newState);
	UFUNCTION(BlueprintCallable, Category = Stats)
		void DamagePlayer(float damage);
	UFUNCTION(BlueprintCallable, Category = Combat)
		void MeleeAttack(float pitch); //This will probably be moved elsewhere

		virtual void TakeDmg(float damage, bool isStatus);

	const int maxInventorySize = 15;

protected:
	UFUNCTION(BlueprintCallable, Category = Stats)
		void SetStats();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Combat)
		void RegenShields(bool shouldRegen);

private:
	UWorld* world;
};
