// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UWeaponBase::UWeaponBase()
{
	luckyMag = 0;
	luckyRnd = 0;
	dmgMult = 1;
	multShot = 1;
	shock = 0;
	freeze = 0;
	explosive = 0;
	rage = 0;
	numMods = 0;
}

UWeaponBase::UWeaponBase(ACharacterBase* newOwner)
{
	Owner = newOwner;
	GEngine->AddOnScreenDebugMessage(3, 20, FColor::Emerald, "Revolver");
	luckyMag = 0;
	luckyRnd = 0;
	dmgMult = 1;
	multShot = 1;
	shock = 0;
	freeze = 0;
	explosive = 0;
	rage = 0;
	numMods = 0;
}

UWeaponBase::UWeaponBase(WeaponType weapon)
{
	gunType = weapon;
	luckyMag = 0;
	luckyRnd = 0;
	dmgMult = 1;
	multShot = 1;
	shock = 0;
	freeze = 0;
	explosive = 0;
	rage = 0;
	numMods = 0;
}

void UWeaponBase::Shoot(FVector muzzleLoc, FVector dir)
{
	if (currentAmmo > 0)
	{
		//float damageDealt;
		float damageDealt;

		if(FMath::RandRange(0, 100) > luckyMag)
			currentAmmo--;

		for (int i = 0; i < numProjectiles * multShot; i++)
		{
			//GEngine->AddOnScreenDebugMessage(3, 2, FColor::Emerald, dir.ToCompactString());
			dir.X += FMath::FRandRange((accuracy * -1), accuracy);
			dir.Y += FMath::FRandRange((accuracy * -1), accuracy);
			dir.Z += FMath::FRandRange((accuracy * -1), accuracy);

			World->LineTraceSingleByChannel(hit, muzzleLoc, ((dir * maxRange) + muzzleLoc), ECC_Pawn, collisionParams);
			DrawDebugLine(World, muzzleLoc, ((dir * maxRange) + muzzleLoc), FColor::Emerald, true, -1, 0, 1);

			if (hit.GetActor() != nullptr && hit.GetActor()->IsValidLowLevel())
			{
				if (hit.GetActor()->ActorHasTag("Enemy") || hit.GetActor()->ActorHasTag("Mob"))
				{
					//GEngine->AddOnScreenDebugMessage(3, 2, FColor::Emerald, World->GetName());
					damageDealt = CalculateDamage(FVector::Dist(hit.Location, muzzleLoc));
					Cast<ACharacterBase>(hit.GetActor())->TakeDmg(damageDealt);
				}
			}
		}
	}
}

void UWeaponBase::SetWeaponStats(float newDamage, float newRange, int newAccuracy, bool newFullAuto)
{
	damage = newDamage;
	accuracy = newAccuracy;
	isFullAuto = newFullAuto;

	if (newRange > maxRange)
		newRange = maxRange;
	range = newRange;
	dmgFallOff = maxRange - range;
}

void UWeaponBase::ReloadGun()
{
	if (currentAmmo != maxAmmo && reserveAmmo != 0)
	{
		if (maxAmmo - currentAmmo > reserveAmmo)
		{
			currentAmmo += reserveAmmo;
			reserveAmmo = 0;
		}
		else
		{
			reserveAmmo -= (maxAmmo - currentAmmo);
			currentAmmo = maxAmmo;
		}
	}
}

TArray<float> UWeaponBase::GetStats()
{
	TArray<float>stats;
	stats.SetNum(5);
	//float stats[5];

	stats[0] = damage;
	stats[1] = range;
	stats[2] = dmgFallOff;
	stats[3] = accuracy;
	stats[4] = maxAmmo;

	return stats;
}

TArray<float> UWeaponBase::GetModifiers()
{
	TArray<float> mods;

	mods.Add(luckyMag);		// [0] Chance the final damage is doubled after calculating
	mods.Add(luckyRnd);		// [1] Chance ammo is not consumed when shooting
	mods.Add(dmgMult);		// [2] Multiplies the final damage when calculating damage
	mods.Add(multShot);		// [3] Increases shots fired without lowering ammo
	mods.Add(shock);		// [4] Temporarily applies Damage over time effect to enemies hit
	mods.Add(freeze);		// [5] Temporarily slows down enemies hit
	mods.Add(rage);			// [6] Increases final damage if under %35 hp
	mods.Add(explosive);	// [7] Bullets explode dealing splash damage

	return mods;
}

FString UWeaponBase::ToString(bool isModifer)
{
	FString newString;

	if (isModifer)
	{
		newString =
			"LuckyMag: " + FString::SanitizeFloat(luckyMag) + "\n" +
			"LuckyRnd: " + FString::SanitizeFloat(luckyRnd) + "\n" +
			"DmgMult: " + FString::SanitizeFloat(dmgMult) + "\n" +
			"MultiShot: " + FString::SanitizeFloat(multShot) + "\n" +
			"Shock: " + FString::SanitizeFloat(shock) + "\n" +
			"Freeze: " + FString::SanitizeFloat(freeze) + "\n" +
			"Rage: " + FString::SanitizeFloat(rage) + "\n" +
			"Explosive: " + FString::SanitizeFloat(explosive);
	}
	else
	{
		newString =
			"Damage: " + FString::SanitizeFloat(damage) + "\n" +
			"Range: " + FString::SanitizeFloat(range) + "\n" +
			"Falloff: " + FString::SanitizeFloat(dmgFallOff) + "\n" +
			"Accuracy: " + FString::SanitizeFloat(accuracy) + "\n" +
			"Ammo: " + FString::SanitizeFloat(currentAmmo + reserveAmmo);
	}

	return newString;
}

void UWeaponBase::ResetWeapon(WeaponType newType, ACharacterBase* newOwner)
{
	Owner = newOwner;
	collisionParams.AddIgnoredActor(Owner);

	SetBaseStats(newType);
	ResetModifiers();
}

void UWeaponBase::ResetModifiers()
{
	//Resets Modifiers to default values
	luckyMag = 0;
	luckyRnd = 0;
	dmgMult = 1;
	multShot = 1;
	shock = 0;
	freeze = 0;
	explosive = 0;
	rage = 0;
	numMods = 0;
}

void UWeaponBase::ResetAmmo()
{
	currentAmmo = maxAmmo;
	reserveAmmo = maxAmmo * 2;
}

void UWeaponBase::SetBaseStats(WeaponType newType)
{
	gunType = newType;

	//The shotdelay and damage has been set so each gun does on around 75 dmg a second
	//Dps = (damage * (100 / (shotDelay * 100)) * numProjectiles)
	//Base value stats for weapons
	switch (gunType)
	{
	case(Shotgun):
		damage = 5;
		range = 1500;
		accuracy = 0.1;
		maxAmmo = 8;
		isFullAuto = false;
		numProjectiles = 5;
		shotDelay = (1.0/3.0);
		//GEngine->AddOnScreenDebugMessage(0, 20, FColor::Emerald, "Shotgun");
		//Set Starting Stats
		break;
	case(Pistol):
		damage = 15;
		range = 1500;
		accuracy = 0.02;
		maxAmmo = 12;
		isFullAuto = false;
		numProjectiles = 1;
		shotDelay = 0.2;
		//GEngine->AddOnScreenDebugMessage(1, 20, FColor::Emerald, "Pistol");
		//Set Starting Stats
		break;
	case(AssaultRifle):
		damage = 7.5;
		range = 1500;
		accuracy = 0.03;
		maxAmmo = 25;
		isFullAuto = true;
		numProjectiles = 1;
		shotDelay = 0.1;
		//GEngine->AddOnScreenDebugMessage(2, 20, FColor::Emerald, "AssaultRifle");
		//Set Starting Stats
		break;
	case(Revolver):
		damage = 18.75;
		range = 1500;
		accuracy = 0.01;
		maxAmmo = 6;
		isFullAuto = false;
		numProjectiles = 1;
		shotDelay = 0.25;
		//GEngine->AddOnScreenDebugMessage(3, 20, FColor::Emerald, "Revolver");
		//Set Starting Stats
		break;
	}
	dmgFallOff = maxRange - range;
	currentAmmo = maxAmmo;
	reserveAmmo = maxAmmo * 2;
}

void UWeaponBase::SetDamage(float newDamage)
{
	damage = newDamage;
}

void UWeaponBase::SetRange(float newRange)
{
	if (newRange > maxRange)
		newRange = maxRange;

	range = newRange;

	dmgFallOff = maxRange - range;
}

void UWeaponBase::SetFallOff(float newFallOff)
{
	dmgFallOff = newFallOff;
}

void UWeaponBase::SetAccuracy(int newAccuracy)
{
	accuracy = newAccuracy;
}

void UWeaponBase::SetModifier(int modId)
{
	numMods++;

	switch (modId)
	{
		case(0):
			luckyRnd++;
			break;
		case(1):
			luckyMag += 2;
			break;
		case(2):
			dmgMult += 0.05;
			break;
		case(3):
			multShot += 2;
			break;
		case(4):
			shock += 4;
			break;
		case(5):
			freeze += 10;
			break;
		case(6):
			rage += 5;
			break;
		case(7):
			explosive = 25;
			break;
	}
}

void UWeaponBase::SetOwner(ACharacterBase* newOwner)
{
	Owner = newOwner;
	collisionParams.AddIgnoredActor(Owner);
}

float UWeaponBase::GetDamage()
{
	return damage;
}

float UWeaponBase::CalculateDamage(float dist)
{
	float finalDamage = damage;

	if (dmgFallOff != 0 && dist > range)
	{
		finalDamage = (1 - ((dist - range) / dmgFallOff)) * damage;
	}

	if ((Owner->maxHealth / Owner->health) <= (Owner->maxHealth * 0.35))
		finalDamage += rage;

	if (FMath::RandRange(0, 100) < luckyRnd)
		finalDamage *= 2; 

	return finalDamage * dmgMult;
}