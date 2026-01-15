// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UserButton.generated.h"

/**
 * 
 */
UCLASS()
class DESCENT_API UUserButton : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (ExposeOnSpawn = "true"))
	FButtonStyle style;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (ExposeOnSpawn = "true"))
	FText text;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (ExposeOnSpawn = "true"))
	FSlateFontInfo font;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (ExposeOnSpawn = "true"))
	FSlateBrush textStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (ExposeOnSpawn = "true"))
	FSlateColor textColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (ExposeOnSpawn = "true"))
	bool shouldWrap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (ExposeOnSpawn = "true"))
	float wrapTextAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (ExposeOnSpawn = "true"))
	FMargin textPadding;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (ExposeOnSpawn = "true"))
	FSlateBrush borderBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (ExposeOnSpawn = "true"))
	FMargin borderPadding;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateText();

	void SetText(FText newText);

protected:

	virtual void NativeConstruct() override;
};
