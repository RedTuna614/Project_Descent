// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameManager.h"
#include "InventoryBase.generated.h"

/**
 * 
 */
UCLASS()
class DESCENT_API UInventoryBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		TArray<UWeaponBase*> playerInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		TArray<UWeaponBase*> playerWeapons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		bool viewShiftProperty;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Inventory)
		void RemoveUI();
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void SetInvUI(TArray<UTextBlock*> invText, UVerticalBox* invContainer);
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void SetWeaponUI(TArray<UTextBlock*> gunText);
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void SwapWeapons(UWeaponBase* invWeapon, UWeaponBase* equipWeapon);
	UFUNCTION(BlueprintCallable, Category = Inventory)
		int GetHoveredWeaponIndex(UVerticalBox* wpnContainer, UVerticalBox* invContainer, bool &inInventory);

protected:

	virtual void NativeConstruct() override;
};
