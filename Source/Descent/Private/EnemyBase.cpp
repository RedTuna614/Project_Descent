// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBase.h"
#include "EnemyWeapon.h"
#include "CoverBase.h"
#include <Kismet/KismetSystemLibrary.h>

AEnemyBase::AEnemyBase() 
{
	state = Wake;
	weapon = NewObject<UEnemyWeapon>();
	GetActorForwardVector();
}

AActor* AEnemyBase::FindCover(bool isFleeing, bool &didFind)
{
	AActor* cover = nullptr;
	FVector actorLoc = GetActorLocation();
	float radius = 1000;
	float coverDist;
	TArray<FHitResult> HitActors;
	TArray<ACoverBase*> coverActors;
	FCollisionShape shape = FCollisionShape::MakeSphere(radius);
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);
	//DrawDebugSphere(GetWorld(), GetActorLocation(), shape.GetSphereRadius(), 32, FColor::Emerald, true);
	GetWorld()->SweepMultiByChannel(HitActors, actorLoc, actorLoc + radius, FQuat::Identity, ECC_Visibility, shape, collisionParams);
	if(!HitActors.IsEmpty())
	{
		for (FHitResult hit : HitActors)
		{
			if (hit.GetActor()->ActorHasTag("Cover"))
			{
				coverActors.Add(Cast<ACoverBase>(hit.GetActor()));
			}
		}
		//cover = coverActors[0];
		//coverDist = FVector::Dist(actorLoc, cover->GetActorLocation());
		coverDist = -1;
		if (isFleeing)
		{
			for (ACoverBase* hitCover : coverActors)
			{
				if (!hitCover->isOccupied)
				{
					if (FVector::Dist(actorLoc, hitCover->GetActorLocation()) > coverDist || (coverDist == -1))
					{
						cover = hitCover;
						coverDist = FVector::Dist(actorLoc, cover->GetActorLocation());
					}
				}
			}
		}
		else
		{
			for (ACoverBase* hitCover : coverActors)
			{
				if (!hitCover->isOccupied)
				{
					if (FVector::Dist(actorLoc, hitCover->GetActorLocation()) < coverDist)
					{
						cover = hitCover;
						coverDist = FVector::Dist(actorLoc, cover->GetActorLocation());
					}
				}
			}
		}

		didFind = (cover != nullptr);
		return cover;
	}
	else
	{
		didFind = false;
		return nullptr;
	}
		
}

void AEnemyBase::SetEnemyStats(EnemyType newEnemy)
{
	if (enemy != newEnemy || enemy == NULL)
		enemy = newEnemy;
	if (weapon == nullptr)
	{
		weapon = NewObject<UEnemyWeapon>();
		weapon->World = GetWorld();
	}
		
	switch (enemy)
	{
		case(Charger):
			movementSpeed = 600;
			health = 75;
			attackDelay = .15;
			break;
		case(Bomber):
			movementSpeed = 500;
			health = 25;
			break;
		case(Mortar):
			movementSpeed = 200;
			health = 75;
			break;
		case(Grunt):
			movementSpeed = 550;
			health = 90;
			attackDelay = .3;
			break;
		case(Elite):
			movementSpeed = 650;
			health = 125;
			attackDelay = .15;
			break;
	}

	weapon->SetEnemyWeaponStats(this);

}
/*
void AEnemyBase::DamageEnemy(float damage)
{
	health -= damage;
	UpdateEnemyState(Damage);

	if (health <= 0)
		Destroy();
}
*/
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

}