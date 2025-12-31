// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "EnemyWeapon.h"
#include "CoverBase.h"
#include "GameManager.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "Engine/OverlapResult.h"

AEnemyBase::AEnemyBase() 
{
	state = Wake;
	weapon = NewObject<UEnemyWeapon>();
	GetCharacterMovement()->MaxWalkSpeed = movementSpeed;
	canExplode = true;
	exploding = false;
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	SetEnemyStats(enemy);
	//UpdateEnemyState(Idle);
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
	UGameManager* gameManager = Cast<UGameManager>(GetGameInstance());
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
			if (gameManager->dungeonMods[4])
			{
				movementSpeed = 600;
				health = 70;
				attackDelay = .15;
			}
			else
			{
				movementSpeed = 400;
				health = 90;
				attackDelay = .3;
			}
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
		case(Engineer):
			movementSpeed = 700;
			health = 50;
			break;
	}

	if (gameManager->dungeonMods[0])
	{
		maxShields = maxHealth * 0.25;
		shields = maxShields;
	}

	weapon->SetEnemyWeaponStats(this);

}

void AEnemyBase::TakeDmg(float damage, bool isStatus)
{
	if (shields != 0)
	{
		if (damage > shields)
		{
			damage -= shields;
			shields = 0;
		}
		else
		{
			shields -= damage;
			damage = 0;
		}
	}
	health -= damage;
	if(health >= 0 && !isStatus)
		UpdateEnemyState(Damage);
	else
	{
		Cast<UGameManager>(GetGameInstance())->UpdateScore(100, false);
		UpdateEnemyState(Death);
	}
}

void AEnemyBase::Explode()
{
	if (!canExplode)
		return;

	UWorld* world = GetWorld();
	AActor* overlapedActor; //Actor overlapped or hit
	FQuat blastQuat = GetActorQuat(); //Quaternion of actor
	FVector blastOrigin = GetActorLocation();
	TArray<FOverlapResult> outOverlaps; //Array of overlapped actors
	FCollisionShape shape = FCollisionShape::MakeSphere(750); //Shape of collision
	FCollisionQueryParams collisionParams; //Additional params for Overlap and Sweep
	collisionParams.AddIgnoredActor(this);

	canExplode = false;
	exploding = true;

	world->OverlapMultiByChannel(outOverlaps, blastOrigin, blastQuat, ECC_Pawn, shape, collisionParams);

	for (FOverlapResult &overlap : outOverlaps)
	{
		overlapedActor = overlap.GetActor();
		if (ShouldExpDamage(overlapedActor, collisionParams))
		{
			//Some actors become null when they get here (This is bad)
			if (overlapedActor != nullptr && overlapedActor->IsValidLowLevel())
				Cast<ACharacterBase>(overlapedActor)->TakeDmg(BlastDmgOffset(overlapedActor), false);
		}
	}
	
	if (Cast<UGameManager>(GetGameInstance())->dungeonMods[3])
		world->SpawnActor<AActor>(napalm, blastOrigin, GetActorRotation());

	Destroy();
}

bool AEnemyBase::ShouldExpDamage(AActor* target, FCollisionQueryParams &colParams)
{
	UWorld* world = GetWorld();
	FHitResult outHit;
	FVector blastOrigin = GetActorLocation();
	FVector endLoc;
	float traceOffset = -50;

	if (IsValid(target) && target != nullptr)
	{
		endLoc = target->GetActorLocation();
		blastOrigin.Z += traceOffset;
		endLoc.Z += traceOffset;

		for (int i = 0; i < 3; i++)
		{
			if (world->LineTraceSingleByChannel(outHit, blastOrigin, endLoc, ECC_Camera, colParams))
			{
				if (outHit.GetActor() == target)
				{
					colParams.AddIgnoredActor(target);
					if (target->ActorHasTag("Enemy"))
					{
						if (!Cast<AEnemyBase>(target)->exploding && Cast<AEnemyBase>(target)->state != Death)
							return true;

						return false;
					}
					return (target->ActorHasTag("Player") || target->ActorHasTag("Mob"));
				}
			}

			blastOrigin.Z += 50;
			endLoc.Z += 50;
		}
	}

	return false;
}

float AEnemyBase::BlastDmgOffset(AActor* hitActor)
{
	FVector targetLoc = hitActor->GetActorLocation();
	float damage = 150;
	float offset;

	offset = 1 - (FVector::Dist(GetActorLocation(), targetLoc) / 750);
	damage *= offset;

	return damage;
}

FVector AEnemyBase::FindMortarMoveLoc(FVector &normal)
{
	FVector upVec = GetActorUpVector();
	FVector actorLoc = GetActorLocation();
	FCollisionQueryParams colParams;
	FHitResult outHit;
	FVector target;

	colParams.AddIgnoredActor(this);

	upVec.X += FMath::Clamp(FMath::FRandRange(-0.5, 0.5), -1, 1);
	upVec.Y += FMath::Clamp(FMath::FRandRange(-0.5, 0.5), -1, 1);
	upVec.Z += FMath::Clamp(FMath::FRandRange(-0.5, 0.5), -1, 1);

	GetWorld()->LineTraceSingleByChannel(outHit, actorLoc, ((upVec * 15000) + actorLoc), ECC_WorldStatic, colParams);
	target = outHit.Location;
	normal = outHit.ImpactNormal;

	return target;
}