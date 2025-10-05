// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterBase.h"
#include "WeaponAssembler.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType) //Different Weapon Types
enum WeaponType : int
{
	Pistol UMETA(DisplayName = "Pistol"),
	Revolver UMETA(DisplayName = "Revolver"),
	Shotgun UMETA(DisplayName = "Shotgun"),
	AssaultRifle UMETA(DisplayName = "Assault Rifle")
	
};

UENUM(BlueprintType) //Different Damage types
enum GunDamageType : int
{
	Energy UMETA(DisplayName = "Enegry"),
	Kinetic UMETA(DisplayName = "Kinetic")
};

UCLASS(Blueprintable, BlueprintType)
class DESCENT_API UWeaponBase : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UWeaponBase();
	UWeaponBase(ACharacterBase* newOwner);
	UWeaponBase(WeaponType weapon);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		TEnumAsByte<WeaponType> gunType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		TEnumAsByte<GunDamageType> dmgType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		TArray<FWeaponPart> weaponParts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) //Max ammo allowed per mag
		int maxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) //Remaining Ammo not loaded in the gun
		int reserveAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) //Ammo loaded in the gun (Current Mag)
		int currentAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float shotDelay; //Replace later with recoil anim (Move to Enemy Weapon when replacing)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		bool isFullAuto;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		bool isBarrelMag;

	UWorld* World;
	UClass* explosionClass;
	int numMods;

	UFUNCTION(BlueprintCallable, Category = Weapon)
		void Shoot(FVector muzzleLoc, FVector dir);
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void SetWeaponStats(float newDamage, float newRange, int newAccuracy, bool newFullAuto);
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void ReloadGun();
	UFUNCTION(BlueprintCallable, Category = Weapon)
		TArray<float>GetStats();
	UFUNCTION(BlueprintCallable, Category = Weapon)
		TArray<float>GetModifiers();
	UFUNCTION(BlueprintCallable, Category = Weapon) //Returns a weapon's Stats or Modifiers as a String
		FString ToString(bool isModifier);

	void ResetWeapon(WeaponType newType, ACharacterBase* newOwner);
	void ResetModifiers();
	void ResetAmmo();
	void SetBaseStats(WeaponType newType);
	void SetDamage(float newDamage);
	void SetRange(float newRange);
	void SetFallOff(float newFallOff);
	void SetAccuracy(int newAccuracy);
	void SetModifier(int modId);
	void SetOwner(ACharacterBase* newOwner);

	void GenWeaponParts(UWorld* world);

	float GetDamage();
	//Finds the actual damage being dealt after falloff and mods 
	float CalculateDamage(float dist);
	float GetModifier(int modId);

protected: 
	float damage; //Weapons Based Damage
	float range; //Distance until damage falloff
	float dmgFallOff; //Rate the damage fallsoff
	float accuracy; //Slight offset when firing to mimic accuracy/recoil

	int numProjectiles; //Number of projects fired in single shot

	float const maxRange = 5000;

	ACharacterBase* Owner;
	FCollisionQueryParams collisionParams;
	FHitResult hit;

	//Modifiers
	int luckyRnd;	// [0] Chance the final damage is doubled after calculating
	float luckyMag; // [1] Chance ammo is not consumed when shooting
	float dmgMult;	// [2] Multiplies the final damage when calculating damage
	int multShot;	// [3] Increases shots fired without lowering ammo
	int shock;		// [4] Temporarily applies Damage over time effect to enemies hit
	int freeze;		// [5] Temporarily slows down enemies hit
	int rage;		// [6] Increases final damage if under %35 hp
	int explosive;	// [7] Bullets explode dealing splash damage
};
