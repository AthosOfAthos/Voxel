// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "UnrealNetwork.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "Math/FastNoise.h"
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

	//FastNoise mynoise;
	FastNoise* TheNoise;

	void Init(int PosX, int PosY, int PosZ, FastNoise* noise);
	
	uint16 GetBlock(int VoxelX, int VoxelY, int VoxelZ);
	void SetBlock(int VoxelX, int VoxelY, int VoxelZ, int Id);

	void Generate();
	int Noise(int VoxelX, int VoxelY, int VoxelZ);
	int Shape(int VoxelX, int VoxelY, int VoxelZ);

	UFUNCTION()
	virtual void OnRep_NetworkData();

	//Logic for updating chunks
	void UpdateChunk();

	//The version of the chunk replicated to clients
	UPROPERTY(ReplicatedUsing = OnRep_NetworkData)
		uint16 NetworkData[1000];

		//TArray<uint16> NetworkData;

	//Blocks stored in an easy to use 3d array
	uint16 ChunkData[10][10][10];

	//tldr what actualy 'spawns' in the world
	uint16 RenderData[10][10][10];

	//Position in chunk grid
	UPROPERTY(Replicated)
		int PosX;
	UPROPERTY(Replicated)
		int PosY;
	UPROPERTY(Replicated)
		int PosZ;


};
