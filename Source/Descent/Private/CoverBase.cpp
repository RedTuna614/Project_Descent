// Fill out your copyright notice in the Description page of Project Settings.

#include "CoverBase.h"

// Sets default values
ACoverBase::ACoverBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	isOccupied = false;
}