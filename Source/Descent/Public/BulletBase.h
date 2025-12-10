// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.h"
#include "GameManager.h"
#include "BulletBase.generated.h"

UCLASS()
class DESCENT_API ABulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
		UGameManager* gameManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		AEnemyBase* shooter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		FVector bulletLoc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float baseRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float baseDamage;
	
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void SetBullet(bool isDecoy);
	UFUNCTION(BlueprintCallable, Category = Weapon)
		float CalcDamage(float dist, bool useMaxRange);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	const int maxRange = 5000;
};
