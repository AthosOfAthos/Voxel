// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voxel_Chunk.h"
#include "GameFramework/Actor.h"
#include "Voxel_World.generated.h"

UCLASS()
class VOXEL_API AVoxel_World : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVoxel_World();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TMap<FString, AVoxel_Chunk*> ChunkMap;
	
};
