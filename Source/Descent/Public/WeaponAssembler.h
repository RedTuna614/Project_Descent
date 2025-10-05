// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponAssembler.generated.h"

UENUM(BlueprintType) //Different Weapon Types
enum PartType : int
{
	Chasis UMETA(DisplayName = "Chasis"),
	Stock UMETA(DisplayName = "Stock"),
	Barrel UMETA(DisplayName = "Barrel"),
	Magazine UMETA(DisplayName = "Magazine")
};

USTRUCT(Blueprintable, BlueprintType)
struct FWeaponPart
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TEnumAsByte<PartType> part;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	UStaticMesh* partMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int partIndex;

};

UCLASS(Blueprintable, BlueprintType)
class DESCENT_API UWeaponAssembler : public UObject
{
	GENERATED_BODY()

public:

	UWeaponAssembler();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TArray<UStaticMesh*>chasisMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TArray<UStaticMesh*>barrelMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TArray<UStaticMesh*>stockMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TArray<UStaticMesh*>magazineMeshes;

	bool barrelMag; //Checks if the magazine should use the barrel(1) or chasis socket(0)
	int fireType; //Id for which WeaponType the parts use
	//FireTypes Ids
	/*
	* 0 - Pistol
	* 1 - Revolver
	* 2 - Shotgun
	* 3 - AssaultRifle
	*/

	TArray<FWeaponPart> AssembleWeapon();
	FWeaponPart MakePart(PartType newPart, int index, UStaticMesh* mesh);
	FWeaponPart MakePart(PartType newPart, int index);

};
