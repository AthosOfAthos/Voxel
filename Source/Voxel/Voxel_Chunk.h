// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "UnrealNetwork.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "Math/FastNoise.h"
#include "Pillars.h"
#include "Thread_GenerateBase.h"
#include "Voxel_Chunk.generated.h"

UCLASS()
class VOXEL_API AVoxel_Chunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVoxel_Chunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//todo make an array of this shit
	UInstancedStaticMeshComponent* GenericVoxel;
	TArray<UInstancedStaticMeshComponent*> VoxelMesh;

	Pillars skp = Pillars();

	Thread_GenerateBase* Chunk_GenerateBase;
	FRunnableThread* TestThread;
	
	uint16 GetBlock(int VoxelX, int VoxelY, int VoxelZ);
	UFUNCTION( NetMulticast, Reliable )
	void SetBlock(int VoxelX, int VoxelY, int VoxelZ, int Id);
	bool IsLocalOccluded(int VoxelX, int VoxelY, int VoxelZ);
	
	UFUNCTION()
	void SetChunkData();
	void UpdateChunk();
	void SlowUpdateChunk();

	int16 SlowUpdatePos = 0;

	UPROPERTY( Replicated ) 
	bool ChunkChanged = false;
	bool IsSlowUpdate = false;
	bool NeedsUpdate = false;

	uint16 ChunkData[27000];

	uint16 RenderData[27000];

	//Position in chunk grid
	int PosX;
	int PosY;
	int PosZ;
};
