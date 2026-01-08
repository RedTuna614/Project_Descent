// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class DESCENT_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats) //Max amount of health a character can have
		float maxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float movementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		TArray<UClass*>statuses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat) //Stores current shields
		float shields;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat) //Needed to ensure shields don't recharge past max
		float maxShields;

	UFUNCTION(BlueprintCallable, Category = Stats)
		virtual void TakeDmg(float damage, bool isStatus);
	UFUNCTION(BlueprintCallable, Category = Stats)
		float GetHealthPercent();
	UFUNCTION(BlueprintCallable, Category = Status)
		void ApplyStatusEffect(UClass* statusClass, int strength);

		void ApplyStatusEffect(int statusType, int strength);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
