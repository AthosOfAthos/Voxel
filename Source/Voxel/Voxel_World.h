// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "UnrealNetwork.h"
#include "Voxel_Chunk.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "Math/FastNoise.h"
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

	FString GetChunkKey(int ChunkX, int ChunkY, int ChunkZ);

	void ManageChunks();
	void LoadChunk(int ChunkX, int ChunkY, int ChunkZ);
	void UnloadChunk(int ChunkX, int ChunkY, int ChunkZ);

	uint16 GetBlock(int VoxelX, int VoxelY, int VoxelZ);
	void SetBlock(int VoxelX, int VoxelY, int VoxelZ, int Id);

	TArray<UStaticMesh*> VoxelMesh;

	TArray<FVector> PlayerLocations;

	TMap<FString, AVoxel_Chunk*> ChunkMap;

	FastNoise perlin;

	int ViewRadius = 5;
	
};
