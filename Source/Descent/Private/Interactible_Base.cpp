// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactible_Base.h"

// Sets default values
AInteractible_Base::AInteractible_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	playerOverlapped = false;

}

// Called when the game starts or when spawned
void AInteractible_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractible_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

