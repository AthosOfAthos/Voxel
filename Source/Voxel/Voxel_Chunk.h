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

	//FastNoise mynoise;
	FastNoise* TheNoise;

	void Init(int PosX, int PosY, int PosZ, FastNoise* noise);
	
	uint16 GetBlock(int BlockX, int BlockY, int BlockZ);

	void Generate();

	UFUNCTION()
	virtual void OnRep_NetworkData();

	bool IsOccluded(int VoxelX, int VoxelY, int VoxelZ);

	//Logic for updating chunks
	void UpdateChunk();

	//The version of the chunk replicated to clients
	UPROPERTY(ReplicatedUsing = OnRep_NetworkData)
		uint16 NetworkData[999];

		//TArray<uint16> NetworkData;

	//Blocks stored in an easy to use 3d array
	uint16 ChunkData[9][9][9];

	//tldr what actualy 'spawns' in the world
	uint16 RenderData[9][9][9];

	//Position in chunk grid
	UPROPERTY(Replicated)
		int PosX;
	UPROPERTY(Replicated)
		int PosY;
	UPROPERTY(Replicated)
		int PosZ;


};
