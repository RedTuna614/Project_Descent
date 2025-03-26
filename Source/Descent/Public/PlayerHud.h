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
	UPROPERTY(BlueprintReadWrite, Category = UI)
	UMaterialInstanceDynamic* shieldMat;

	UFUNCTION(BlueprintCallable, Category = UI)
	void UpdateDamageMat(float hp, float maxHP);
	UFUNCTION(BlueprintCallable, Category = UI)
	void UpdateShieldMat(float shield, float maxShield);
	UFUNCTION(BlueprintCallable, Category = UI)
	void HideDamageMat(bool shouldHide);
	UFUNCTION(BlueprintCallable, Category = UI)
	void HideShieldMat(bool shouldHide);


};
