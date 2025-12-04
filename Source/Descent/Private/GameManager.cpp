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
}

void UGameManager::ResetVars()
{
	levelSize = baseLevelSize;
	difficulty = 1;
	playerInventory.Empty();
	playerWeapons.Empty();
	gameScore.Empty();
	gameScore.Init(0, 2);
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