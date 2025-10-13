// Fill out your copyright notice in the Description page of Project Settings.


#include "Loot_Chest.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

ALoot_Chest::ALoot_Chest()
{
	newWeapon = NewObject<UWeaponBase>();
	player = nullptr;
}

UWeaponBase* ALoot_Chest::CreateWeapon()
{
	/*
	float* baseStats = playerWeapon->GetBaseStats(playerWeapon->gunType);
	float* currStats = playerWeapon->GetStats();
	float newStats[5] = { 0 };
	float statDif = 0;
	
	/*
	* [0] = damage
	* [1] = range
	* [2] = dmgFalloff
	* [3] = accuracy
	* [4] = maxAmmo
	

	for (int i = 0; i < 5; i++)
	{
		statDif = currStats[i] - baseStats[i];
		newStats[i] = FMath::RandRange(baseStats[i])
	}
	*/
	UWeaponBase* playerWeapon = player->EquippedWeapons[FMath::RandRange(0,1)];
	UWeaponBase* weapon = NewObject<UWeaponBase>();
	UWorld* world = GetWorld();
	int currMods = playerWeapon->numMods;
	int numNewMods = FMath::RandRange(1, currMods + 2);
	int weaponId = FMath::RandRange(0, 3);
	int modId;

	//weapon->SetBaseStats(StaticCast<WeaponType>(weaponId));
	weapon->GenWeaponParts(world);
	weapon->ResetModifiers();

	for (int i = 0; i < numNewMods; i++)
	{
		modId = FMath::RandRange(0, 7);
		weapon->SetModifier(modId);
		weapon->SetOwner(player);
	}
	if (weapon->GetModifier(7) != 0)
		weapon->explosionClass = explosive;
	//GEngine->AddOnScreenDebugMessage(21, 20, FColor::Emerald, "WeaponMade");

	return weapon;
}

void ALoot_Chest::BeginPlay()
{
	Super::BeginPlay();

	newWeapon = NewObject<UWeaponBase>();
	newWeapon->ResetModifiers();
}