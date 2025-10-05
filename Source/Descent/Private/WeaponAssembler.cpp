// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAssembler.h"

UWeaponAssembler::UWeaponAssembler() {}

TArray<FWeaponPart> UWeaponAssembler::AssembleWeapon()
{
	TArray<FWeaponPart> weapon;
	int len;
	int index = 0;

	len = chasisMeshes.Num();
	index = FMath::RandRange(0, len - 1);
	weapon.Add(MakePart(Chasis, index, chasisMeshes[index]));

	len = stockMeshes.Num();
	index = FMath::RandRange(0, len);
	//Allows weapons to spawn without stocks (pistols/revovlers)
	if (index != len)
	{
		weapon.Add(MakePart(Stock, index, stockMeshes[index]));
		//Update MaxRange as more weaponTypes are added
		fireType = FMath::RandRange(2, 3);
	}
	else
	{
		fireType = 0; //Weapon is pistol
		weapon.Add(MakePart(Stock, index));
	}

	len = barrelMeshes.Num();
	index = FMath::RandRange(0, len - 1);
	weapon.Add(MakePart(Barrel, index, barrelMeshes[index]));

	//Barrels 2 and 3 can have magazines attached to them
	if ((index == 2 || index == 3) && FMath::RandBool())
	{
		barrelMag = true;
		switch (index)
		{
		case(2):
			index = 0;
			break;
		case(3):
			index = 1;
			break;
		}
	}
	else
	{
		barrelMag = false;
		switch (weapon[0].partIndex)
		{
		case(0):
			index = 2;
			break;
		case(1):
			index = 3;
			break;
		case(2):
			index = 4;
			break;
		}
	}
	weapon.Add(MakePart(Magazine, index, magazineMeshes[index]));

	return weapon;
}

FWeaponPart UWeaponAssembler::MakePart(PartType newPart, int index, UStaticMesh* mesh)
{
	FWeaponPart weaponPart;
	weaponPart.part = newPart;
	weaponPart.partIndex = index;
	weaponPart.partMesh = mesh;

	return weaponPart;
}

FWeaponPart UWeaponAssembler::MakePart(PartType newPart, int index)
{
	FWeaponPart weaponPart;
	weaponPart.part = newPart;
	weaponPart.partIndex = index;

	return weaponPart;
}