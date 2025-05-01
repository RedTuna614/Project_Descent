// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType) //Different Weapon Types
enum WeaponType : int
{
	Shotgun UMETA(DisplayName = "Shotgun"),
	Pistol UMETA(DisplayName = "Pistol"),
	AssaultRifle UMETA(DisplayName = "Assault Rifle"),
	Revolver UMETA(DisplayName = "Revolver")
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
	UWeaponBase(WeaponType weapon);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		TEnumAsByte<WeaponType> gunType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		TEnumAsByte<GunDamageType> dmgType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		USkeletalMesh* body;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		USkeletalMesh* stock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		USkeletalMesh* barrel;
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

	UWorld* World;

	UFUNCTION(BlueprintCallable, Category = Weapon)
		void Shoot(FVector muzzleLoc, FVector dir);
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void SetWeaponStats(float newDamage, float newRange, int newAccuracy, bool newFullAuto);
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void ReloadGun();

	void ResetWeapon(WeaponType newType, AActor* owner);
	void SetDamage(float newDamage);
	void SetRange(float newRange);
	void SetFallOff(float newFallOff);
	void SetAccuracy(int newAccuracy);

	float GetDamage();
	//Finds the actual damage being dealt after falloff and mods 
	float CalculateDamage(float dist);

protected: 
	float damage; //Weapons Based Damage
	float range; //Distance until damage falloff
	float dmgFallOff; //Rate the damage fallsoff
	int accuracy; 

	float const maxRange = 5000;

	FCollisionQueryParams collisionParams;
	FHitResult hit;

	//Modifiers
	float luckyRnd; //Chance the final damage is doubled after calculating
	float luckyMag; //Chance ammo is not consumed when shooting
	float dmgMult; //Multiplies the final damage when calculating damage
	int multShot; //Increases shots fired without lowering ammo
	bool shock; //Temporarily applies Damage over time effect to enemies hit
	bool freeze; //Temporarily slows down enemies hit
	bool rage; //Increases final damage if under %50 hp
	bool explosive; //Bullets explode dealing splash damage
};
