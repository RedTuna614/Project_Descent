// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactible_Base.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

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

	collider->OnComponentBeginOverlap.AddDynamic(this, &AInteractible_Base::BeginOverlap);
	collider->OnComponentEndOverlap.AddDynamic(this, &AInteractible_Base::EndOverlap);

	player = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AInteractible_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (playerOverlapped)
		if (player->isInteracting)
		{
			Interaction();
		}
}

void AInteractible_Base::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
		playerOverlapped = true;
}

void AInteractible_Base::EndOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player"))
		playerOverlapped = false;
}

void AInteractible_Base::Interaction_Implementation()
{

}
