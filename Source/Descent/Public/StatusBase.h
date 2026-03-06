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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Status)
		ACharacter* Owner;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Duration)
		TArray<int> stacks;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Duration)
		int stackNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Duration)
		int duration;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Status)
		void StatusEffect(int strength, ACharacterBase* attachedTo);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Status)
		void AddStatus(int strength);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;		
};
