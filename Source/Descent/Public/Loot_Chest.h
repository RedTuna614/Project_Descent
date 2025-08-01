// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactible_Base.h"
#include "Loot_Chest.generated.h"

/**
 * 
 */
UCLASS()
class DESCENT_API ALoot_Chest : public AInteractible_Base
{
	GENERATED_BODY()
	
public:
	ALoot_Chest();

	UWeaponBase* newWeapon;

	UFUNCTION(BlueprintCallable, Category = Loot)
		UWeaponBase* CreateWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
