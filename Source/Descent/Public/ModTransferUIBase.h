// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "InventoryBase.h"
#include "WeaponBase.h"
#include "PlayerBase.h"
#include "ModTransferUIBase.generated.h"

/**
 * 
 */
UCLASS()
class DESCENT_API UModTransferUIBase : public UInventoryBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Button)
		TArray<UWidget*> modSelectBtns;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
		TArray<UTextBlock*> lModVals;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
		TArray<UTextBlock*> rModVals;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reference)
		APlayerBase* player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		UWeaponBase* lostWpn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		UWeaponBase* targetWpn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reference, meta = (ExposeOnSpawn = "true"))
		AActor* Owner;


	UFUNCTION(BlueprintCallable, Category = Weapon)
		void SwapMods();
	UFUNCTION(BlueprintCallable)
		void SetModSelect();
	UFUNCTION(BlueprintCallable, Category = Button)
		void SetLActiveButton(UUserButton* newButton);
	UFUNCTION(BlueprintCallable, Category = Button)
		void ResetActiveButton();

protected:

	virtual void NativeConstruct() override;

	UUserButton* LActiveButton;

};
