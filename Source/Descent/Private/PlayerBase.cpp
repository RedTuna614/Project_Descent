// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerBase.h"
#include "GameManager.h"
#include "Engine/OverlapResult.h"
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

void APlayerBase::SetStats()
{
	UGameManager* gameManager = Cast<UGameManager>(GetGameInstance());
	//GEngine->AddOnScreenDebugMessage(5, 2, FColor::Red, GetWorld()->GetName());
	world = GetWorld();
	Inventory = gameManager->playerInventory;
	for (UWeaponBase* weapon : Inventory)
	{
		weapon->World = world;
		weapon->SetOwner(this);
	}

	if (gameManager->playerWeapons.IsEmpty())
	{
		//EquippedWeapons = { NewObject<UWeaponBase>(), NewObject<UWeaponBase>() };
		for (int i = 0; i < EquippedWeapons.Num(); i++)
		{
			if (EquippedWeapons[i] == nullptr)
			{
				EquippedWeapons[i] = NewObject<UWeaponBase>();
				//GEngine->AddOnScreenDebugMessage(3, 2, FColor::Emerald, "Bob");
			}
		}
		EquippedWeapons[0]->ResetWeapon(Pistol, this);
		EquippedWeapons[1]->ResetWeapon(AssaultRifle, this);
		gameManager->playerWeapons = EquippedWeapons;
	}
	else
		EquippedWeapons = gameManager->playerWeapons;

	for (int i = 0; i < EquippedWeapons.Num(); i++)
	{
		EquippedWeapons[i]->World = world;
		EquippedWeapons[i]->SetOwner(this);
		EquippedWeapons[i]->ResetAmmo();
	}

	movementSpeed = 600;
	shields = 200;
	maxShields = shields;
	maxHealth = 150;
	if (gameManager->dungeonMods[13])
		health = gameManager->playerHealth;
	else
		health = maxHealth;
}

void APlayerBase::UpdateState(PlayerStates newState)
{
	if (state != newState)
	{
		state = newState;
		switch (state) 
		{
			case(Idl):
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

	if (!Cast<UGameManager>(GetGameInstance())->canPlayerDie)
	{
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
				//UGameplayStatics::OpenLevel(GetWorld(), "Main");
				HUD->ShowDeathUI();
			}
		}
		else
		{
			shields -= damage;
			HUD->HideShieldMat(false);
			HUD->UpdateShieldMat(shields, maxShields);
		}
	}
}

void APlayerBase::MeleeAttack(float pitch)
{
	FHitResult outHit;
	FCollisionQueryParams queryParams;
	AActor* hitActor;
	FVector startPos, dir, offsetDir;

	dir = GetActorForwardVector();
	offsetDir.Z = pitch;
	startPos = GetActorLocation();
	queryParams.AddIgnoredActor(this);

	for (float i = -0.5; i < 0.5; i += 0.02)
	{
		offsetDir.X = FMath::Clamp(i + dir.X, -1, 1);
		offsetDir.Y = FMath::Clamp(i + dir.Y, -1, 1);
		//DrawDebugLine(world, startPos, ((200 * offsetDir) + startPos), FColor::Red, true, -1, 0, 1);
		if (world->LineTraceSingleByChannel(outHit, startPos, (startPos + (200 * offsetDir)), ECC_Camera, queryParams))
		{
			hitActor = outHit.GetActor();
			if (hitActor != nullptr && IsValid(hitActor))
			{
				if (hitActor->ActorHasTag("Enemy") || hitActor->ActorHasTag("Mob"))
				{
					Cast<ACharacterBase>(hitActor)->TakeDmg(75, false);
					DrawDebugLine(world, startPos, ((200 * offsetDir) + startPos), FColor::Red, true, -1, 0, 1);
					return;
				}
			}
		}
	}
	
}

void APlayerBase::TakeDmg(float damage, bool isStatus)
{
	
	DamagePlayer(damage);
}