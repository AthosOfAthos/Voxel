// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "UnrealNetwork.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "Voxel_Chunk.generated.h"

UCLASS()
class VOXEL_API AVoxel_Chunk : public AActor
{
	GENERATED_BODY()
	
public:	
	AVoxel_Chunk();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	TArray<UInstancedStaticMeshComponent*> VoxelMesh;
	
	void FindThread();

	uint16 GetBlock(int VoxelX, int VoxelY, int VoxelZ);
	UFUNCTION( NetMulticast, Reliable )
	void SetBlock(int VoxelX, int VoxelY, int VoxelZ, int Id);
	bool IsLocalOccluded(int VoxelX, int VoxelY, int VoxelZ);
	
	void UpdateChunk();

	UPROPERTY( Replicated ) 
	bool ChunkChanged = false;
	bool NeedsUpdate = false;
	bool NeedsGeneration = true;
	bool HasGenerationTask = false;
	int GenerationThreadID;

	uint16 ChunkData[27000];

	uint16 RenderData[27000];

	//Position in chunk grid
	int PosX;
	int PosY;
	int PosZ;
};
