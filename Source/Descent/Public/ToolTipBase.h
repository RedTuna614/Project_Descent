// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ToolTipBase.generated.h"

/**
 * 
 */
UCLASS()
class DESCENT_API UToolTipBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ToolTip)
		FString toolTipString = "";
};
