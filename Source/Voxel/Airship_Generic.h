// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Airship_Generic.generated.h"

UCLASS()
class VOXEL_API AAirship_Generic : public AActor
{
	GENERATED_BODY()
	
public:	
	AAirship_Generic();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UStaticMeshComponent* AirShipMesh;

	UPROPERTY( Replicated )
	FRotator Attitude;
	UPROPERTY(Replicated)
	float AirSpeed;

};
