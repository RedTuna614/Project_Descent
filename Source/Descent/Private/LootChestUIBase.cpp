// Fill out your copyright notice in the Description page of Project Settings.


#include "LootChestUIBase.h"
#include "PlayerBase.h"
#include "Components/TextBlock.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void ULootChestUIBase::NativeConstruct()
{
	Super::NativeConstruct();
}

bool ULootChestUIBase::TakeWeapon(int index, int &outIndex)
{
	outIndex = index;
	UWorld* world = GetWorld();
	APlayerBase* player = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(world, 0));

	if (player->Inventory.Num() < player->maxInventorySize)
	{
		player->Inventory.Add(weaponChoices[index]);
		return true;
	}

	return false;
}

void ULootChestUIBase::ReplaceWeapon(int newIndex, int oldIndex, bool isInInventory)
{
	UWorld* world = GetWorld();
	FInputModeGameAndUI input;
	APlayerBase* player = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(world, 0));

	if (isInInventory)
		player->Inventory[oldIndex] = weaponChoices[newIndex];
	else
		player->EquippedWeapons[oldIndex] = weaponChoices[newIndex];

	UGameplayStatics::GetPlayerController(world, 0)->SetInputMode(input);
	RemoveFromParent();
}

void ULootChestUIBase::SetWeaponChoices(TArray<UTextBlock*> wpnText)
{
	int len = wpnText.Num();
	FString name;

	for (int i = 0; i < len; i++)
	{
		weaponChoices.Add(Owner->CreateWeapon());
		switch (weaponChoices[i]->gunType)
		{
		case(Shotgun):
			name = "Shotgun";
			break;
		case(Pistol):
			name = "Pistol";
			break;
		case(AssaultRifle):
			name = "AssaultRifle";
			break;
		case(Revolver):
			name = "Revolver";
			break;
		}
		wpnText[i]->SetText(FText::FromString(name));
	}
}