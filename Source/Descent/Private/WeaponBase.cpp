// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "CharacterBase.h"

UWeaponBase::UWeaponBase()
{

}

UWeaponBase::UWeaponBase(WeaponType weapon)
{
	gunType = weapon;
}

void UWeaponBase::Shoot(FVector muzzleLoc, FVector dir)
{
	if (currentAmmo > 0)
	{
		//float damageDealt;
		float damageDealt = 0;

		currentAmmo--;

		World->LineTraceSingleByChannel(hit, muzzleLoc, ((dir * maxRange) + muzzleLoc), ECC_Pawn, collisionParams);
		DrawDebugLine(World, muzzleLoc, ((dir * maxRange) + muzzleLoc), FColor::Emerald, true, -1, 0, 1);

		if (hit.GetActor() != nullptr && hit.GetActor()->IsValidLowLevel())
		{
			if (hit.GetActor()->ActorHasTag("Enemy"))
			{
				if (FVector::Dist(hit.Location, muzzleLoc) > range)
				{
					if (dmgFallOff != 0)
						damageDealt = (1 - ((FVector::Dist(hit.Location, muzzleLoc) - range) / dmgFallOff)) * damage;
					else
						damageDealt = damage;
				}
				else
				{
					damageDealt = damage;
				}
				Cast<ACharacterBase>(hit.GetActor())->TakeDmg(damageDealt);
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

void UWeaponBase::ResetWeapon(WeaponType newType, AActor* owner)
{
	gunType = newType;
	//The shotdelay and damage has been set so each gun does on around 75 dmg a second
	//Dps = damage * (100 / (shotDelay * 100))
	switch (gunType)
	{
		case(Shotgun):
			damage = 25;
			range = 1500;
			accuracy = 0;
			maxAmmo = 8;
			isFullAuto = false;
			shotDelay = 0.3;
			//GEngine->AddOnScreenDebugMessage(0, 20, FColor::Emerald, "Shotgun");
			//Set Starting Stats
			break;
		case(Pistol):
			damage = 15;
			range = 1500;
			accuracy = 0;
			maxAmmo = 12;
			isFullAuto = false;
			shotDelay = 0.2;
			//GEngine->AddOnScreenDebugMessage(1, 20, FColor::Emerald, "Pistol");
			//Set Starting Stats
			break;
		case(AssaultRifle):
			damage = 7.5;
			range = 1500;
			accuracy = 0;
			maxAmmo = 25;
			isFullAuto = true;
			shotDelay = 0.1;
			GEngine->AddOnScreenDebugMessage(2, 20, FColor::Emerald, "AssaultRifle");
			//Set Starting Stats
			break;
		case(Revolver):
			damage = 18.75;
			range = 1500;
			accuracy = 0;
			maxAmmo = 6;
			isFullAuto = false;
			shotDelay = 0.25;
			GEngine->AddOnScreenDebugMessage(3, 20, FColor::Emerald, "Revolver");
			//Set Starting Stats
			break;
	}
	dmgFallOff = maxRange - range;
	currentAmmo = maxAmmo;
	reserveAmmo = maxAmmo * 2;
	
	collisionParams.AddIgnoredActor(owner);

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

float UWeaponBase::GetDamage()
{
	return damage;
}