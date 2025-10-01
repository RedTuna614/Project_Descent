// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponPart.generated.h"

UENUM(BlueprintType) //Different Weapon Types
enum PartType : int
{
	Chasis UMETA(DisplayName = "Chasis"),
	Barrel UMETA(DisplayName = "Barrel"),
	Stock UMETA(DisplayName = "Stock"),
	Magazine UMETA(DisplayName = "Magazine")
};

UCLASS(Blueprintable, BlueprintType)
class DESCENT_API UWeaponPart : public UObject
{
	GENERATED_BODY()
	
public:

	UWeaponPart();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		TArray<UStaticMesh*> partMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		TEnumAsByte<PartType> part;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		UStaticMesh* partMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int fireType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		bool hasEmissive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		bool barrelMag; //Does the magazine attach to the barrel

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void UpdateMaterial();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Weapon)
		UWeaponPart* AssembleWeapon();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Weapon)
		int SetFireType();

};
