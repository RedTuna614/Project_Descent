// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHud.h"

void UPlayerHud::UpdateDamageMat(float hp, float maxHP)
{
	damageMat->SetScalarParameterValue("Radius", hp / maxHP);
}

void UPlayerHud::UpdateShieldMat(float shield, float maxShield)
{
	shieldMat->SetScalarParameterValue("Alpha", shield / maxShield);
}

void UPlayerHud::HideDamageMat(bool shouldHide)
{
	damageMat->SetScalarParameterValue("isHidden", shouldHide);
}

void UPlayerHud::HideShieldMat(bool shouldHide)
{
	shieldMat->SetScalarParameterValue("isHidden", shouldHide);
}