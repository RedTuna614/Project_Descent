// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHud.h"

void UPlayerHud::UpdateDamageMat(float hp, float maxHP)
{
	damageMat->SetScalarParameterValue("Radius", hp / maxHP);
}