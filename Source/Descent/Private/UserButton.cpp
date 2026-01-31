// Fill out your copyright notice in the Description page of Project Settings.


#include "UserButton.h"

void UUserButton::NativeConstruct()
{
	Super::NativeConstruct();

	isSelected = false;
}

void UUserButton::SetText(FText newText)
{
	text = newText;
	UpdateText();
	//Text_Block->SetText(text);
}

void UUserButton::Select_UnselectButton()
{
	if (isSelected)
	{
		isSelected = false;
		style.Normal.TintColor = FColor::FromHex("#082811A0");
	}
	else
	{
		isSelected = true;
		style.Normal.TintColor = FColor::FromHex("385444A0");
		//FColor::FromHex("385444A0")
	}
	UpdateStyle();
}