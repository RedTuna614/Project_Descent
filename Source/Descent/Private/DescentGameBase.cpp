// Fill out your copyright notice in the Description page of Project Settings.


#include "DescentGameBase.h"

ADescentGameBase::ADescentGameBase()
{
	numKillRooms = 0;
	numChambers = 0;
	didWin = false;
}

void ADescentGameBase::BeginPlay()
{
	UGameManager* gameManager = Cast<UGameManager>(GetGameInstance());
	if (gameManager->debugMod && gameManager->activeMods == 0)
	{
		for (int& debug : gameManager->modsToDebug)
		{
			gameManager->inactiveMods.Remove(debug);
			gameManager->dungeonMods[debug] = true;
		}

		gameManager->activeMods = gameManager->modsToDebug.Num();
	}
}

void ADescentGameBase::ProgressGoal()
{
	goalNum--;
	if (goalNum == 0)
	{
		didWin = true;
		GEngine->AddOnScreenDebugMessage(8, 5, FColor::Red, "Victory");
	}
}

void ADescentGameBase::SetGoal(MissionType mission)
{
	switch (mission)
	{
	case(Kill):
		goalNum = 2 * numKillRooms;
		break;
	case(Find):
		goalNum = numChambers * .5;
		break;
	}
}

void ADescentGameBase::ChangeToMapView(bool inMap)
{
	for (ARoomBase* room : roomsSpawned)
	{
		room->ChangeMapColor(inMap);
	}
}