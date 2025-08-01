// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryBase.h"
#include "PlayerBase.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void UInventoryBase::NativeConstruct()
{
	Super::NativeConstruct();

	UGameManager* gameManager = Cast<UGameManager>(GetGameInstance());
	APlayerBase* player = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	playerInventory = player->Inventory;
	playerWeapons = player->EquippedWeapons;
}

void UInventoryBase::SetInvUI(TArray<UTextBlock*> invText, UVerticalBox* invContainer)
{
	if (playerInventory.IsEmpty())
	{
		for (UTextBlock* text : invText)
			text->SetText(FText::FromString(""));
		invContainer->SetRenderOpacity(35);
		invContainer->SetIsEnabled(false);
		GEngine->AddOnScreenDebugMessage(24, 5, FColor::Blue, "Empty Inventory");
	}
	else
	{
		TArray<UWidget*> invSlot = invContainer->GetAllChildren();
		int len = playerInventory.Num();
		int i;
		FString name;

		invContainer->SetRenderOpacity(100);
		invContainer->SetIsEnabled(true);

		for (i = 0; i < len; i++)
		{
			invSlot[i]->SetIsEnabled(true);
			invSlot[i]->SetRenderOpacity(100);
			switch (playerInventory[i]->gunType)
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
			invText[i]->SetText(FText::FromString(name));
		}
		if (len != invText.Num())
		{
			for (i = invText.Num() - 1; i >= len; i--)
			{
				invText[i]->SetText(FText::FromString(""));
				invSlot[i]->SetIsEnabled(false);
				invSlot[i]->SetRenderOpacity(35);
			}
		}
	}
}

void UInventoryBase::SetWeaponUI(TArray<UTextBlock*> gunText)
{
	FString name;
	for (int i = 0; i < playerWeapons.Num(); i++)
	{
		switch (playerWeapons[i]->gunType)
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
		gunText[i]->SetText(FText::FromString(name));
	}
}

void UInventoryBase::SwapWeapons(UWeaponBase* invWeapon, UWeaponBase* equipWeapon)
{
	APlayerBase* player = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	int index;

	playerInventory.Find(invWeapon, index);
	player->Inventory[index] = equipWeapon;

	playerWeapons.Find(equipWeapon, index);
	GEngine->AddOnScreenDebugMessage(14, 5, FColor::Red, FString::SanitizeFloat(index));
	player->EquippedWeapons[index] = invWeapon;
}

int UInventoryBase::GetHoveredWeaponIndex(UVerticalBox* wpnContainer, UVerticalBox* invContainer, bool &inInventory)
{
	TArray<UWidget*> containerChildren;
	int len;

	if (invContainer->IsHovered() && invContainer->GetIsEnabled())
	{
		containerChildren = invContainer->GetAllChildren();
		inInventory = true;
	}
	else if (wpnContainer->IsHovered() && wpnContainer->GetIsEnabled())
	{
		containerChildren = wpnContainer->GetAllChildren();
		inInventory = false;
	}
	else
		return -1;

	len = containerChildren.Num();

	for (int i = 0; i < len; i++)
	{
		if (containerChildren[i]->IsHovered())
		{
			return i;
		}
	}

	return -1;
}