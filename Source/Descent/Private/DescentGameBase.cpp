// Fill out your copyright notice in the Description page of Project Settings.


#include "DescentGameBase.h"

ADescentGameBase::ADescentGameBase()
{
	numKillRooms = 0;
	numTreasureRooms = 0;
	didWin = false;
}

void ADescentGameBase::BeginPlay()
{

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
		goalNum = 7 * (numKillRooms * .5);
		break;
	case(Find):
		goalNum = numTreasureRooms * .5;
		break;
	}
}