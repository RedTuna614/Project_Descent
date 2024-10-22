// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "EnemyBase.h"
#include "EnemyWeapon.generated.h"

/**
 * 
 */
UCLASS()
class DESCENT_API UEnemyWeapon : public UWeaponBase
{
	GENERATED_BODY()

public:

	UEnemyWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		UClass* projectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float meleeDelay;

	UFUNCTION(BlueprintCallable, Category = Weapon)
		void SetEnemyWeaponStats(AEnemyBase* owner);
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void RangedAttack(FVector muzzleLoc, FRotator rot);
	UFUNCTION(BlueprintCallable, Category = Weapon)
		AActor* MeleeAttack(ACharacterBase* Attacker, FVector targetLoc, float &damageDealt);
	UFUNCTION(BlueprintCallable, Category = Weapon)
		float GetDamage();
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void SetShotCount();
	
	AEnemyBase* Enemy;

};
