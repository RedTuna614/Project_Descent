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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reference)
		APlayerBase* player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		UWeaponBase* lostWpn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		UWeaponBase* targetWpn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reference, meta = (ExposeOnSpawn = "true"))
		AActor* Owner;

	UFUNCTION(BlueprintCallable, Category = Weapon)
		void SwapMods(int newModId);
	UFUNCTION(BlueprintCallable)
		void SetModSelect(UWeaponBase* weapon, TArray<UTextBlock*> modText);

protected:

	virtual void NativeConstruct() override;

};
