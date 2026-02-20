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

	AddToViewport();

}

void UModTransferUIBase::SwapMods()
{
	UGameManager* gameManager = Cast<UGameManager>(GetGameInstance());
	int num = 0;
	int modId = 0;

	for (bool mod : lostWpn->hasMod)
	{
		if (mod)
		{
			num = lostWpn->modLevel[modId];
			for (int i = 0; i < num; i++)
				targetWpn->SetModifier(modId);
		}
		modId++;
	}

	//Ensures the player always has two EquippedWeapons
	if (playerWeapons.Find(lostWpn, num))
	{
		if (!playerWeapons.Contains(targetWpn))
		{
			playerWeapons[num] = targetWpn;
			player->EquippedWeapons[num] = targetWpn;
			lostWpn = targetWpn;
		}
		else
		{
			playerWeapons[num] = playerInventory[0];
			player->EquippedWeapons[num] = playerInventory[0];
			lostWpn = playerInventory[0];
		}
	}

	player->Inventory.Remove(lostWpn);
}

void UModTransferUIBase::SetModSelect()
{
	TArray<int>modVals;
	int len = lModVals.Num();
	modVals.Init(0, len);

	if (lostWpn != nullptr)
	{
		modVals = lostWpn->modLevel;
		for (int i = 0; i < len; i++)
			lModVals[i]->SetText(FText::AsNumber(modVals[i]));
	}
	else
		for (UTextBlock* text : lModVals)
			text->SetText(FText::AsNumber(000));
	if (targetWpn != nullptr)
	{
		int n = 0;
		for (int i = 0; i < len; i++)
		{
			modVals[i] += targetWpn->modLevel[i];
			rModVals[i]->SetText(FText::AsNumber(modVals[i]));
		}
	}
	else
		for (UTextBlock* text : rModVals)
			text->SetText(FText::AsNumber(000));
}

void UModTransferUIBase::SetLActiveButton(UUserButton* newButton)
{
	if (LActiveButton != nullptr)
		LActiveButton->Select_UnselectButton();
	LActiveButton = newButton;
	LActiveButton->Select_UnselectButton();
}

void UModTransferUIBase::ResetActiveButton()
{
	activeButton->Select_UnselectButton();
	activeButton = nullptr;
	LActiveButton->Select_UnselectButton();
	LActiveButton = nullptr;
}