// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryBase.h"
#include "WeaponBase.h"
#include "ModUpgrade.generated.h"

/**
 * 
 */
UCLASS()
class DESCENT_API UModUpgrade : public UInventoryBase
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = Mod)
		UWeaponBase* ModifyWeapon(UWeaponBase* weapon, bool isUpgrade);
};
