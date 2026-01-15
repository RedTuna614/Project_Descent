// Fill out your copyright notice in the Description page of Project Settings.


#include "UserButton.h"

void UUserButton::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUserButton::SetText(FText newText)
{
	text = newText;
	UpdateText();
	//Text_Block->SetText(text);
}