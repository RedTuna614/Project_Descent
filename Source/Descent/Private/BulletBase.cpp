// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBase.h"

// Sets default values
ABulletBase::ABulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
	
	bulletLoc = GetActorLocation();

	SetBullet(GetOwner()->ActorHasTag("Enemy"));

	//GEngine->AddOnScreenDebugMessage(10, 2, FColor::Emerald, "Sheep");

	SetLifeSpan(5);
}

void ABulletBase::SetBullet(bool isDecoy)
{
	if (isDecoy)
	{
		baseRange = 650;
		baseDamage = 5;
	}
	else
	{
		shooter = Cast<AEnemyBase>(GetOwner());
		switch (shooter->enemy)
		{
		case(Grunt):
			baseRange = 750;
			baseDamage = 5;
			break;
		case(Elite):
			baseRange = 1500;
			baseDamage = 10;
			break;
		case(Sniper):
			baseDamage = 25;
		}
	}
}

float ABulletBase::CalcDamage(float dist, bool useMaxRange)
{
	float damageDealt = baseDamage;

	if (useMaxRange)
	{
		if (dist > baseRange)
		{
			float fallOff = maxRange - baseRange;

			damageDealt = (1 - ((dist - baseRange) / fallOff)) * baseDamage;
		}

		return damageDealt;
	}
	else
	{
		if (dist > baseRange)
			return 0;
		else
		{
			damageDealt = (1 - (dist / baseRange)) * baseDamage;
		}

		return damageDealt;
	}
}