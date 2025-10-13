// Fill out your copyright notice in the Description page of Project Settings.


#include "ModTransferUIBase.h"
#include "Components/VerticalBox.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/Engine/Classes/Kismet/KismetTextLibrary.h>

void UModTransferUIBase::NativeConstruct()
{
	Super::NativeConstruct();

	player = Cast<APlayerBase>(GetOwningPlayerPawn());
	playerInventory = player->Inventory;
	playerWeapons = player->EquippedWeapons;
}

void UModTransferUIBase::SwapMods(int newModId)
{
	UGameManager* gameManager = Cast<UGameManager>(GetGameInstance());
	int num = lostWpn->modLevel[newModId];

	for (int i = 0; i < num; i++)
	{
		targetWpn->SetModifier(newModId);
	}

	//Ensures the player always has two EquippedWeapons
	if (playerWeapons.Find(lostWpn, num))
	{
		if (!playerWeapons.Contains(targetWpn))
		{
			playerWeapons[num] = targetWpn;
			lostWpn = targetWpn;
		}
		else
		{
			playerWeapons[num] = playerInventory[0];
			lostWpn = playerInventory[0];
		}
	}

	player->Inventory.Remove(lostWpn);
}

void UModTransferUIBase::SetModSelect(UWeaponBase* weapon, TArray<UTextBlock*> modText)
{
	TArray<float>modVals;
	int len = modText.Num();

	if (weapon != nullptr)
	{
		modVals = weapon->GetModifiers();
		//Sets the text for the mod values in the UI
		for (int i = 0; i < len; i++)
		{
			modText[i]->GetParent()->SetIsEnabled(weapon->hasMod[i]);

			if (weapon->hasMod[i])
				modText[i]->SetText(FText::FromString(FString::SanitizeFloat(modVals[i])));
			else
				modText[i]->SetText(FText::FromString("0.0"));
		}
	}
	else
	{
		for (UTextBlock* text : modText)
		{
			text->SetText(FText::FromString("0.0"));
			text->GetParent()->SetIsEnabled(false);
		}
	}

	if (lostWpn == nullptr || targetWpn == nullptr)
		for (UWidget* btn : modSelectBtns)
			btn->SetIsEnabled(false);
	else
	{
		len = modSelectBtns.Num();
		for (int i = 0; i < len; i++)
			modSelectBtns[i]->SetIsEnabled(lostWpn->hasMod[i]);
	}

}