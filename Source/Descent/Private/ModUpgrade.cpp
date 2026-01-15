// Fill out your copyright notice in the Description page of Project Settings.


#include "ModUpgrade.h"
#include "PlayerBase.h"

UWeaponBase* UModUpgrade::ModifyWeapon(int index, bool isEquipped, bool isUpgrade)
{
	TArray<int>modAvail;
	UWeaponBase* weapon;
	if (isEquipped)
		weapon = playerWeapons[index];
	else
		weapon = playerInventory[index];

	for (int i = 0; i < weapon->hasMod.Num(); i++)
	{
		if (weapon->hasMod[i] == isUpgrade)
			modAvail.Add(i);
	}

	index = modAvail[FMath::RandRange(0, modAvail.Num() - 1)];
	weapon->SetModifier(index);

	return weapon;
}
