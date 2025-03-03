// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHud.generated.h"

/**
 * 
 */
UCLASS()
class DESCENT_API UPlayerHud : public UUserWidget
{
	GENERATED_BODY()

public:

	//UPlayerHud();

	UPROPERTY(BlueprintReadWrite, Category = UI)
	UMaterialInstanceDynamic* damageMat;

	UFUNCTION(BlueprintCallable, Category = UI)
	void UpdateDamageMat(float hp, float maxHP);

};
