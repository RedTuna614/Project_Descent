// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UGameManager::UGameManager()
{
	levelSize = 0;
	baseLevelSize = levelSize;
	difficulty = 1;
	useInputSize = false;
}