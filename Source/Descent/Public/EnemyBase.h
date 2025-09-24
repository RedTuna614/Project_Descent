// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "NavigationSystem.h"
#include "EnemyBase.generated.h"

class UEnemyWeapon;

UENUM(BlueprintType) //Different Weapon Types
enum EnemyType : int
{
	Charger UMETA(DisplayName = "Charger"),
	Bomber UMETA(DisplayName = "Bomber"),
	Mortar UMETA(DisplayName = "Mortar"),
	Grunt UMETA(DisplayName = "Grunt"),
	Elite UMETA(DisplayName = "Elite"),
	Sniper UMETA(DisplayName = "Sniper"),
	Brute UMETA(DisplayName = "Brute")
};

UENUM(BlueprintType)
enum EnemyState : int
{
	Wake UMETA(DisplayName = "Wake"),
	Idle UMETA(DisplayName = "Idle"),
	Attacking UMETA(DisplayName = "Attacking"),
	Moving UMETA(DisplayName = "Moving"),
	Crouching UMETA(DisplayName = "Crouching"),
	Damage UMETA(DisplayName = "Damage"),
	Death UMETA(DisplayName = "Death"),
	Fleeing UMETA(DisplayName = "Fleeing"),
	MoveNShoot UMETA (DisplayName = "MoveNShoot")
};

UCLASS()
class DESCENT_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()
	
public:

	AEnemyBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
		TEnumAsByte<EnemyType> enemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
		TEnumAsByte<EnemyState> state;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
		TEnumAsByte<EnemyState> lastState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
		ACharacterBase* Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		UEnemyWeapon* weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
		bool inCover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		float attackDelay;

	UFUNCTION(BlueprintCallable, Category = Enemy)
		AActor* FindCover(bool isFleeing, bool &didFind);
	UFUNCTION(BlueprintCallable, Category = Enemy)
		FVector FindFleeToLoc(UNavigationSystemV1* navSystem, bool &Success);
	UFUNCTION(BlueprintCallable, Category = Enemy)
		void SetEnemyStats(EnemyType newEnemy);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Enemy)
		void UpdateEnemyState(EnemyState newState);

		void TakeDmg(float damage, bool isStatus);
	/*
	UFUNCTION(BlueprintCallable, Category = Enemy)
		void DamageEnemy(float damage);*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
