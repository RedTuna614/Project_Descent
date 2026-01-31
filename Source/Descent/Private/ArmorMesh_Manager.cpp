// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorMesh_Manager.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"

// Sets default values
AArmorMesh_Manager::AArmorMesh_Manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}

// Called when the game starts or when spawned
void AArmorMesh_Manager::BeginPlay()
{
	Super::BeginPlay();

	world = GetWorld();
}

void AArmorMesh_Manager::SpawnInstance(UStaticMesh* mesh, FTransform transform)
{
	FMeshId newMesh;
	UStaticMeshComponent* meshComp;

	newMesh.mesh = world->SpawnActor<AStaticMeshActor>(transform.GetLocation(), transform.GetRotation().Rotator());
	meshComp = newMesh.mesh->GetStaticMeshComponent();
	meshComp->SetMobility(EComponentMobility::Movable);
	meshComp->SetStaticMesh(mesh);
	meshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	meshComp->SetSimulatePhysics(true);
	meshComp->SetCanEverAffectNavigation(false);
	newMesh.mesh->SetLifeSpan(2);

	newMesh.time = 2;
}

void AArmorMesh_Manager::ManageInstances()
{
	TArray<FMeshId> manage = staticMeshes;

	for (FMeshId id : staticMeshes)
	{
		id.time--;
		if (id.time == 0)
			id.mesh->Destroy();
	}

	staticMeshes.RemoveAll([](FMeshId id) {return id.time == 0; });

	if (staticMeshes.IsEmpty())
	{
		EndTimer();
	}

}