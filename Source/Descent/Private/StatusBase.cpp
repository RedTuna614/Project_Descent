// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusBase.h"
#include "CharacterBase.h"

// Sets default values for this component's properties
UStatusBase::UStatusBase()
{

}


// Called when the game starts
void UStatusBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UStatusBase::AddStatus_Implementation(int strength)
{
	stacks.Add(duration);
	stackNum++;

	if (stackNum == 1)
		StatusEffect(strength, Cast<ACharacterBase>(Owner));
}