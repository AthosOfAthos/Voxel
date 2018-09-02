// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "UnrealNetwork.h"
#include "App.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "HoverBoard.generated.h"

UCLASS()
class VOXEL_API AHoverBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHoverBoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddPitch(float inputAmount);
	void AddRoll(float inputAmount);

	UFUNCTION(Server, Reliable, WithValidation)
	void AddPitchServer(float inputAmount);
	UFUNCTION(Server, Reliable, WithValidation)
	void AddRollServer(float inputAmount);

	UBoxComponent* BoardCollision;
	UStaticMeshComponent* BoardMesh;

	UPROPERTY(Replicated)
	FRotator BoardRotation;
	UPROPERTY(Replicated)
	float AirSpeed = 0;

	float PitchResponse = 80;
	float RollResponse = 120;
	float YawResponse = 4;
	
};
