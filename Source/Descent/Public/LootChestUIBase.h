// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Loot_Chest.h"
#include "LootChestUIBase.generated.h"

/**
 * 
 */
UCLASS()
class DESCENT_API ULootChestUIBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Loot)
		TArray<UWeaponBase*> weaponChoices;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Loot, meta = (ExposeOnSpawn = "true"))
		ALoot_Chest* Owner;

	UFUNCTION(BlueprintCallable, Category = Loot)
		bool TakeWeapon(int index, int &outIndex);
	UFUNCTION(BlueprintCallable, Category = Loot)
		void ReplaceWeapon(int newIndex, int oldIndex, bool isInInventory);
	UFUNCTION(BlueprintCallable, Category = Loot)
		void SetWeaponChoices(TArray<UTextBlock*> wpnText);

protected:

	virtual void NativeConstruct() override;
};
