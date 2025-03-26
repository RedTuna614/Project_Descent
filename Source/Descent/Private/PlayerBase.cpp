// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerBase.h"
#include "GameManager.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

APlayerBase::APlayerBase() 
{
	shields = 0;
	activeWeapon = 0;
	EquippedWeapons.SetNum(2);
	EquippedWeapons = { NewObject<UWeaponBase>(), NewObject<UWeaponBase>() };
	inCombat = false;
	isInteracting = false;
}

void APlayerBase::SetStats(PlayerClasses newPlayerClass)
{
	//EquippedWeapons = { NewObject<UWeaponBase>(), NewObject<UWeaponBase>() };
	for (int i = 0; i < EquippedWeapons.Num(); i++)
	{
		if (EquippedWeapons[i] == nullptr)
			EquippedWeapons[i] = NewObject<UWeaponBase>();
	}

	if (playerType != newPlayerClass || playerType == NULL)
	{
		playerType = newPlayerClass;
		switch (playerType)
		{
			case(Scout):
				shields = 200;
				health = 150;
				movementSpeed = 600;
				EquippedWeapons[0]->ResetWeapon(Pistol, this);
				EquippedWeapons[0]->World = GetWorld();
				EquippedWeapons[1]->ResetWeapon(AssaultRifle, this);
				EquippedWeapons[1]->World = GetWorld();
				break;
			case(Engineer):
				shields = 200;
				health = 200;
				movementSpeed = 100;
				EquippedWeapons[0]->ResetWeapon(Revolver, this);
				EquippedWeapons[0]->World = GetWorld();
				EquippedWeapons[1]->ResetWeapon(Shotgun, this);
				EquippedWeapons[1]->World = GetWorld();
				break;
			case(Arcist):
				shields = 200;
				health = 100;
				movementSpeed = 150;
				break;
		}
		maxHealth = health;
		maxShields = shields;
	}
}

void APlayerBase::UpdateState(PlayerStates newState)
{
	if (state != newState)
	{
		state = newState;
		switch (state) 
		{
			case(Idle):
				//Do Stuff
				break;
			case(Running):
				//Do Stuff
				break;
			case(Sprinting):
				//Do Stuff
				break;
			case(Damaged):
				//Do Stuff
				break;
		}
	}
}

void APlayerBase::DamagePlayer(float damage)
{
	UpdateState(Damaged);
	RegenShields(false);

	if (shields <= 0)
	{
		health -= damage;
		HUD->HideDamageMat(false);
		HUD->HideShieldMat(true);
		HUD->UpdateDamageMat(health, maxHealth);
		GEngine->AddOnScreenDebugMessage(10, 20, FColor::Emerald, FString::SanitizeFloat(health));

		if (health <= 0)
		{
			//DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			UGameplayStatics::OpenLevel(GetWorld(), "Main");
		}
	}
	else
	{

		shields -= damage;
		HUD->HideShieldMat(false);
		HUD->UpdateShieldMat(shields, maxShields);
	}
}