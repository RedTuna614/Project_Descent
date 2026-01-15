// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Health_Upgrade_UI.generated.h"

/**
 * 
 */
UCLASS()
class DESCENT_API UHealth_Upgrade_UI : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintCallable, Category = Game)
	void RestorePlayerHealth();
	UFUNCTION(BlueprintCallable, Category = Game)
	void RefillPlayerAmmo();
};
