// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerBase.h"
#include "Interactible_Base.generated.h"

UCLASS()
class DESCENT_API AInteractible_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractible_Base();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
		APlayerBase* player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
		bool playerOverlapped;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
		UPrimitiveComponent* collider;

	UFUNCTION(BlueprintCallable, Category = Interaction)
		virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable, Category = Interaction)
		void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintNativeEvent, Category = Interaction)
		void Interaction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
