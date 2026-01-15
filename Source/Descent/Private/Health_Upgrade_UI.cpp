// Fill out your copyright notice in the Description page of Project Settings.


#include "Health_Upgrade_UI.h"
#include "PlayerBase.h"
#include "Kismet/GameplayStatics.h"

void UHealth_Upgrade_UI::RestorePlayerHealth()
{
	APlayerBase* player = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UGameManager* gameManager = Cast<UGameManager>(GetGameInstance());
	
	player->health = player->maxHealth;
	gameManager->playerHealth = player->maxHealth;

	//RemoveFromParent();
}

void UHealth_Upgrade_UI::RefillPlayerAmmo()
{
	APlayerBase* player = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	for (UWeaponBase* weapon : player->Inventory)
	{
		weapon->ResetAmmo();
	}
	for (UWeaponBase* weapon : player->EquippedWeapons)
	{
		weapon->ResetAmmo();
	}

	//RemoveFromParent();
}