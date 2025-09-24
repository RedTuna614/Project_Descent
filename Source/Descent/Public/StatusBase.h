// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class DESCENT_API UStatusBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		int duration;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Status)
		void StatusEffect(int strength, ACharacterBase* attachedTo);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;		
};
