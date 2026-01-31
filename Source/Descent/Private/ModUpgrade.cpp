// Fill out your copyright notice in the Description page of Project Settings.


#include "ModUpgrade.h"
#include "PlayerBase.h"

UWeaponBase* UModUpgrade::ModifyWeapon(UWeaponBase* weapon, bool isUpgrade)
{
	TArray<int>modAvail;
	int index;
	int len = weapon->hasMod.Num();

	for (int i = 0; i < len; i++)
	{
		if (weapon->hasMod[i] == isUpgrade)
			modAvail.Add(i);
	}

	index = modAvail[FMath::RandRange(0, modAvail.Num() - 1)];
	weapon->SetModifier(index);

	return weapon;
}
