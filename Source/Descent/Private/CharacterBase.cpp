// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "StatusBase.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBase::TakeDmg(float damage, bool isStatus)
{
	health -= damage;
	if (health <= 0)
		Destroy();
}

float ACharacterBase::GetHealthPercent()
{
	return health / maxHealth;
}

void ACharacterBase::ApplyStatusEffect(int statusType, int strength)
{
	UStatusBase* status =
		Cast<UStatusBase>(AddComponentByClass(statuses[statusType], false, GetActorTransform(), false));

	status->StatusEffect(strength, this);
}