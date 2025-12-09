// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWeapon.h"
#include "EnemyBase.h"

UEnemyWeapon::UEnemyWeapon() {}

void UEnemyWeapon::SetEnemyWeaponStats(AEnemyBase* owner)
{
	collisionParams.AddIgnoredActor(owner);
	Enemy = owner;
	switch (owner->enemy)
	{
		case Charger:
			//Melee Damage
			damage = 10;
			meleeDelay = .25;
			break;
		case Bomber:
			//Explosion Damage
			damage = 150;
			break;
		case Mortar:
			//Explosion Damage
			shotDelay = 1;
			damage = 200;
			break;
		case Grunt:
			//Melee Damage
			shotDelay = .5;
			damage = 1;
			meleeDelay = 2;
			maxAmmo = 3;
			break;
		case Elite:
			//Melee Damage
			shotDelay = .3;
			damage = 5;
			meleeDelay = 1;
			break;
		case Sniper:
			shotDelay = 1;
			damage = 15;
			break;
		case Brute:
			meleeDelay = .2;
			damage = 15;
			break;
	}
}

void UEnemyWeapon::RangedAttack(FVector muzzleLoc, FRotator rot)
{
	//SetShotCount();

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = Enemy;
	World->SpawnActor<AActor>(projectile, muzzleLoc, rot, spawnParams);
}

AActor* UEnemyWeapon::MeleeAttack(ACharacterBase* Attacker, FVector targetLoc, float& damageDealt)
{
	FVector actorLoc = Attacker->GetActorLocation();
	damageDealt = 0;
	World->LineTraceSingleByChannel(hit, actorLoc, (targetLoc), ECC_Pawn, collisionParams);
	//DrawDebugLine(World, actorLoc, targetLoc, FColor::Emerald, false, 2.0, 0, 1);
	if (hit.GetActor() != nullptr)
	{
		if (hit.GetActor()->ActorHasTag("Player"))
		{
			damageDealt = damage;
			//collisionParams.ClearIgnoredActors();
			return hit.GetActor();
		}
		else
		{
			//collisionParams.ClearIgnoredActors();
			return nullptr;
		}
	}
	else
	{
		//collisionParams.ClearIgnoredActors();
		return nullptr;
	}
}

float UEnemyWeapon::GetDamage()
{
	return damage;
}

void UEnemyWeapon::SetShotCount()
{
	currentAmmo = FMath::RandRange(1, maxAmmo);
}

void UEnemyWeapon::SetDamage(float newDamage)
{
	damage = newDamage;
}