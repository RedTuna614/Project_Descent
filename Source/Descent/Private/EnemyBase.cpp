// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "EnemyWeapon.h"
#include "CoverBase.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/CharacterMovementComponent.h>

AEnemyBase::AEnemyBase() 
{
	state = Wake;
	weapon = NewObject<UEnemyWeapon>();
	GetCharacterMovement()->MaxWalkSpeed = movementSpeed;
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	SetEnemyStats(enemy);
	UpdateEnemyState(Idle);
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
		for (FHitResult& hit : HitActors)
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

FVector AEnemyBase::FindFleeToLoc(UNavigationSystemV1* navSystem, bool &Success)
{
	TArray<FVector> points;
	FNavLocation navPoint;
	FVector actorLoc = GetActorLocation();
	FVector strtLoc = { actorLoc.X - 1500, actorLoc.Y - 1500, actorLoc.Z };
	FVector endLoc = { actorLoc.X + 1500, actorLoc.Y + 1500, actorLoc.Z };
	FVector point = strtLoc;
	int pointAcc = 50;

	for (int y = 0; y < (1500 * 2) / pointAcc; y++)
	{
		for (int x = 0; x < (1500 * 2) / pointAcc; x++)
		{
			point = { strtLoc.X + (pointAcc * x), strtLoc.Y + (pointAcc * y), strtLoc.Z };
			if (navSystem->ProjectPointToNavigation(point, navPoint, { 34, 34, 88 }))
			{
				points.Add(point);
			}
		}
	}

	if (!points.IsEmpty())
	{
		points.Sort([actorLoc](const FVector& x, const FVector& y)
		{	return FVector::Dist(x, actorLoc) < FVector::Dist(y, actorLoc); });
	}
	else
	{
		Success = false;
		return actorLoc;
	}

	Success = true;
	return points.Top();
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
			health = 1;
			break;
		case(Mortar):
			movementSpeed = 200;
			health = 75;
			break;
		case(Grunt):
			movementSpeed = 400;
			health = 90;
			attackDelay = .3;
			break;
		case(Elite):
			movementSpeed = 600;
			health = 125;
			attackDelay = .15;
			break;
		case(Sniper):
			movementSpeed = 650;
			health = 50;
			attackDelay = .4;
			break;
		case(Brute):
			movementSpeed = 75;
			health = 200;
			attackDelay = .5;
			break;
	}

	weapon->SetEnemyWeaponStats(this);

}

void AEnemyBase::TakeDmg(float damage)
{
	health -= damage;
	if (health <= 0)
		UpdateEnemyState(Death);
	else
		UpdateEnemyState(Damage);
}
