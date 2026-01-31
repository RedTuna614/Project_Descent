// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameManager.h"
#include "UserButton.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mod)
		bool doneConstructing;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Inventory)
		void RemoveUI();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = ToolTip)
		void GetToolTip_UI();
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void SetInvUI(TArray<UUserButton*> button, UVerticalBox* invContainer);
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void SetWeaponUI(TArray<UUserButton*> gunText);
	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool SwapWeapons(UWeaponBase* invWeapon, UWeaponBase* equipWeapon);
	UFUNCTION(BlueprintCallable, Category = Inventory)
		int GetHoveredWeaponIndex(UVerticalBox* wpnContainer, UVerticalBox* invContainer, bool &inInventory);
	UFUNCTION(BlueprintCallable, Category = Button)
		void SetActiveButton(UUserButton* newButton);
	UFUNCTION(BlueprintCallable, Category = Button)
		void DeactivateButton();

protected:
	virtual void NativeConstruct() override;

	UUserButton* activeButton;
};
