// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameManager.h"
#include "Interactible_Base.h"
#include "MissionSelectUIBase.generated.h"

/**
 * 
 */
UCLASS()
class DESCENT_API UMissionSelectUIBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = Mission)
		TArray<int> missionRank;
	UPROPERTY(BlueprintReadWrite, Category = Mission)
		int missionsAvailable;
	UPROPERTY(BlueprintReadWrite, Category = Mission, meta = (ExposeOnSpawn = "true"))
		AInteractible_Base* Owner;

	UFUNCTION(BlueprintCallable, Category = Mission)
		void SetMissions(TArray<UPanelWidget*> missionContainers, TArray<UTextBlock*> objText);
	UFUNCTION(BlueprintCallable, Category = Mission)
		void SelectMisison(int missionIndex);
	
protected:

	TArray<MissionType> missions;

	virtual void NativeConstruct() override;

};
