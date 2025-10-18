// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionSelectUIBase.h"
#include "Components/PanelWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void UMissionSelectUIBase::NativeConstruct()
{
	Super::NativeConstruct();

}

void UMissionSelectUIBase::SetMissions(TArray<UPanelWidget*> missionContainers, TArray<UTextBlock*> objText)
{
	TArray<UWidget*>children;
	int rank = 0;
	for (int n = 0; n < missionContainers.Num(); n++)
	{
		children = missionContainers[n]->GetAllChildren();
		rank = FMath::RandRange(0, 0);
		switch (rank)
		{
		case(0):
			missions.Add(Kill);
			objText[n]->SetText(FText::FromString("Kill"));
			break;
			/*
			case(1):
				missions.Add(Find);
				objText[n]->SetText(FText::FromString("Find"));
				break;
			*/
		}
		rank = FMath::RandRange(1, 5);
		missionRank.Add(rank);
		for (int i = 0; i < rank; i++)
		{
			Cast<UImage>(children[i])->SetColorAndOpacity(FColor::Orange);
		}
	}
}

void UMissionSelectUIBase::SelectMisison()
{
	UGameManager* gameManager = Cast<UGameManager>(GetGameInstance());
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	gameManager->objective = nextMission;
	controller->SetInputMode(FInputModeGameOnly());
	controller->SetShowMouseCursor(false);


	if (!gameManager->useInputSize)
	{
		gameManager->levelSize = (gameManager->baseLevelSize * gameManager->difficulty) / 2;
		GEngine->AddOnScreenDebugMessage(15, 10, FColor::Cyan, FString::SanitizeFloat(gameManager->levelSize), true);
	}

	UGameplayStatics::OpenLevel(GetWorld(), "LevelGenTest");
}