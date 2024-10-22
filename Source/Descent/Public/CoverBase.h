// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoverBase.generated.h"

UCLASS()
class DESCENT_API ACoverBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoverBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cover)
		bool isOccupied;

};
