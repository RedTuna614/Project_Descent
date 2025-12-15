// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UGameManager::UGameManager()
{
	levelSize = 0;
	baseLevelSize = levelSize;
	difficulty = 1;
	useInputSize = false;
	gameScore.Init(0, 2);
	activeMods = 0;
	modActivated = false;
}

void UGameManager::ActivateMod()
{
	int modId = FMath::RandRange(0, inactiveMods.Num() - 1);
	activeMods = inactiveMods[modId];
	dungeonMods[activeMods] = true;
	modActivated = true;
	switch (activeMods)
	{
		case(0):
			//Mobs now have shields (Excluding Sniper decoys)X
			break;
		case(1):
			//All enemy projectiles travel fasterX
			break;
		case(2):
			//Changes enemy spawn randomness to favor gruntsX
			break;
		case(3):
			//Exploding enemies leave behind fire that burns the player
			break;
		case(4):
			//Grunts move faster, shoot faster, have less health, charge the player at low health, and explode on deathX
			break;
		case(5):
			//Sniper decoys now shoot at, and damage the player (Should never have shields)X
			break;
		case(6):
			//Chargers now teleport closer to the player if they're too far awayX
			break;
		case(7):
			//Brute enemies move faster and deal more damage the lower their health getsX
			break;
		case(8):
			//Hallways can now spawn with traps (traps should only maim the player not kill)
			break;
		case(9):
			//Crafter enemy can now spawn in rooms
			break;
		case(10):
			//Rocket enemy can now spawn in rooms
			break;
		case(11):
			//Engineer enemy can now spawn in rooms
			break;
		case(12):
			//Lootchest's now contain only one weapon
			break;
	}
	inactiveMods.RemoveAt(modId);
}

void UGameManager::ResetVars()
{
	levelSize = baseLevelSize;
	difficulty = 1;
	playerInventory.Empty();
	playerWeapons.Empty();
	gameScore.Empty();
	gameScore.Init(0, 2);
	activeMods = 0;
	inactiveMods.SetNum(13);
	dungeonMods.Init(false, 13);
	for (int i = 0; i < inactiveMods.Num(); i++)
	{
		dungeonMods[i] = false;
		inactiveMods[i] = i;
	}
}

void UGameManager::UpdateScore(int score, bool completeLevel)
{
	gameScore[0] += score;
	if (completeLevel)
	{
		gameScore[1]++;
		gameScore[0] *= gameScore[1];
	}
}

bool UGameManager::DidActivateMod(int& modId)
{
	modId = activeMods;
	return modActivated;
}