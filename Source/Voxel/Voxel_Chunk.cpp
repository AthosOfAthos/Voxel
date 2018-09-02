// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxel_Chunk.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <UnrealMathUtility.h>


AVoxel_Chunk::AVoxel_Chunk()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	VoxelMesh.Init(nullptr, 5);

	GenericVoxel = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Generic Voxel"));
	GenericVoxel->SetupAttachment(RootComponent);
	VoxelMesh[1] = GenericVoxel;

	GenericVoxel = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Stone"));
	GenericVoxel->SetupAttachment(RootComponent);
	VoxelMesh[2] = GenericVoxel;

	GenericVoxel = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Dirt"));
	GenericVoxel->SetupAttachment(RootComponent);
	VoxelMesh[3] = GenericVoxel;

	GenericVoxel = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Sand"));
	GenericVoxel->SetupAttachment(RootComponent);
	VoxelMesh[4] = GenericVoxel;

	//Find generic Voxel mesh world loaded for us
	VoxelMesh[1]->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_Generic.Voxel_Generic")));
	VoxelMesh[2]->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_Stone.Voxel_Stone")));
	VoxelMesh[3]->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_Dirt.Voxel_Dirt")));
	VoxelMesh[4]->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_Sand.Voxel_Sand")));

	for (int i = 0; i < 27000; i++)
	{
		ChunkData[i] = 0;
		RenderData[i] = 0;
	}
}

//Register variables for replication
void AVoxel_Chunk::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVoxel_Chunk, PosX);
	DOREPLIFETIME(AVoxel_Chunk, PosY);
	DOREPLIFETIME(AVoxel_Chunk, PosZ);

	DOREPLIFETIME(AVoxel_Chunk, ChunkData);
}

// Called when the game starts or when spawned
void AVoxel_Chunk::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{

	}

}

void AVoxel_Chunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		if (Chunk_GenerateBase != nullptr)
		{
			if (Chunk_GenerateBase->IsFinished)
			{
				delete Chunk_GenerateBase;
				Chunk_GenerateBase = nullptr;
				UpdateChunk();
			}
		}

		if (NeedsUpdate)
		{
			NeedsUpdate = false;
			UpdateChunk();
		}
		
		
	}

}

void AVoxel_Chunk::Init(int LocX, int LocY, int LocZ)
{
	if (HasAuthority())
	{
		PosX = LocX;
		PosY = LocY;
		PosZ = LocZ;

		Chunk_GenerateBase = new Thread_GenerateBase();
		Chunk_GenerateBase->Setup(PosX, PosY, PosZ, ChunkData);
		FRunnableThread::Create(Chunk_GenerateBase, TEXT("GenerationThread"), 0, TPri_Normal);
	}
}

uint16 AVoxel_Chunk::GetBlock(int VoxelX, int VoxelY, int VoxelZ)
{
	return ChunkData[(VoxelX % 30) + ((VoxelY % 30) * 30) + ((VoxelZ % 30) * 900)];
}

void AVoxel_Chunk::SetBlock(int VoxelX, int VoxelY, int VoxelZ, int Id)
{
	if (HasAuthority())
	{
		//ChunkData[(VoxelX % 30) + ((VoxelY % 30) * 30) + ((VoxelZ % 30) * 900)] = Id;
		for (int i = 0; i < 27000; i++)
		{
			ChunkData[i] = 0;
		}
		NeedsUpdate = true;
	}
}

bool AVoxel_Chunk::IsLocalOccluded(int VoxelX, int VoxelY, int VoxelZ)
{
	//chunk edges MUST NOT be occluded
	if (VoxelX == 0 || VoxelX == 29 || VoxelY == 0 || VoxelY == 29 || VoxelZ == 0 || VoxelZ == 29)
		return false;

	if (ChunkData[FMath::Clamp(VoxelX + 1, 0, 29) + FMath::Clamp(VoxelY, 0, 29) * 30 + FMath::Clamp(VoxelZ, 0, 29) * 900] == 0)
		return false;
	if (ChunkData[FMath::Clamp(VoxelX - 1, 0, 29) + FMath::Clamp(VoxelY, 0, 29) * 30 + FMath::Clamp(VoxelZ, 0, 29) * 900] == 0)
		return false;
	if (ChunkData[FMath::Clamp(VoxelX, 0, 29) + FMath::Clamp(VoxelY + 1, 0, 29) * 30 + FMath::Clamp(VoxelZ, 0, 29) * 900] == 0)
		return false;
	if (ChunkData[FMath::Clamp(VoxelX, 0, 29) + FMath::Clamp(VoxelY - 1, 0, 29) * 30 + FMath::Clamp(VoxelZ, 0, 29) * 900] == 0)
		return false;
	if (ChunkData[FMath::Clamp(VoxelX, 0, 29) + FMath::Clamp(VoxelY, 0, 29) * 30 + FMath::Clamp(VoxelZ + 1, 0, 29) * 900] == 0)
		return false;
	if (ChunkData[FMath::Clamp(VoxelX, 0, 29) + FMath::Clamp(VoxelY, 0, 29) * 30 + FMath::Clamp(VoxelZ - 1, 0, 29) * 900] == 0)
		return false;
	
	return true;
}

void AVoxel_Chunk::SetChunkData()
{
	UpdateChunk();
}

void AVoxel_Chunk::UpdateChunk()
{
	for (int i = 0; i < 5; i++)
	{
		if (VoxelMesh[i] != nullptr)
			VoxelMesh[i]->ClearInstances();
	}

	for (int8 VoxelX = 0; VoxelX < 30; VoxelX++)
	{
		for (int8 VoxelY = 0; VoxelY < 30; VoxelY++)
		{
			for (int8 VoxelZ = 0; VoxelZ < 30; VoxelZ++)
			{
				if (IsLocalOccluded(VoxelX, VoxelY, VoxelZ))
					RenderData[VoxelX + VoxelY * 30 + VoxelZ * 900] = 0;
				else
					RenderData[VoxelX + VoxelY * 30 + VoxelZ * 900] = ChunkData[VoxelX + VoxelY * 30 + VoxelZ * 900];
			}
		}
	}

	
	for (int8 VoxelX = 0; VoxelX < 30; VoxelX++)
	{
		for (int8 VoxelY = 0; VoxelY < 30; VoxelY++)
		{
			for (int8 VoxelZ = 0; VoxelZ < 30; VoxelZ++)
			{
				int i = RenderData[VoxelX + VoxelY * 30 + VoxelZ * 900];
				if (VoxelMesh[i] != nullptr)
					VoxelMesh[i]->AddInstance(FTransform(FRotator(0, 0, 0), FVector(VoxelX * 100, VoxelY * 100, VoxelZ * 100), FVector(1, 1, 1)));
			}
		}
	}
	
}