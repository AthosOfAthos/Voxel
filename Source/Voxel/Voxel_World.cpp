// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxel_World.h"


// Sets default values
AVoxel_World::AVoxel_World()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	//Load Generic Voxel mesh
	VoxelMesh.Init(nullptr, 5);
	VoxelMesh[1] = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_Generic.Voxel_Generic"));
	VoxelMesh[2] = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_Stone.Voxel_Stone"));
	VoxelMesh[3] = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_Dirt.Voxel_Dirt"));
	VoxelMesh[4] = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_Sand.Voxel_Sand"));

	PlayerLocations.Init(FVector(0, 0, 0), 1);
}


void AVoxel_World::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		//GetWorld()->GetTimerManager().SetTimer(ChunkTimer, this, &AVoxel_World::ManageChunks, 0.6, true, 0);

		//preload spawn
		
		for (int ChunkX = -2; ChunkX < 2; ChunkX++)
		{
			for (int ChunkY = -2; ChunkY < 2; ChunkY++)
			{
				for (int ChunkZ = -2; ChunkZ < 2; ChunkZ++)
				{
					if (!ChunkMap.Contains(GetChunkKey(ChunkX, ChunkY, ChunkZ)))
					{
						LoadChunk(ChunkX, ChunkY, ChunkZ);
					}
				}
			}
		}
		
	}
}

// Called every frame
void AVoxel_World::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		
	}
}

FString AVoxel_World::GetChunkKey(int ChunkX, int ChunkY, int ChunkZ)
{
	FString ChunkKey = FString().SanitizeFloat(ChunkX);
	ChunkKey.Append(",");
	ChunkKey.Append(FString().SanitizeFloat(ChunkY));
	ChunkKey.Append(",");
	ChunkKey.Append(FString().SanitizeFloat(ChunkZ));
	return ChunkKey;
}

void AVoxel_World::ManageChunks()
{
	if (HasAuthority())
	{
		for (int ChunkX = -2; ChunkX < 2; ChunkX++)
		{
			for (int ChunkY = -2; ChunkY < 2; ChunkY++)
			{
				for (int ChunkZ = -2; ChunkZ < 2; ChunkZ++)
				{
					if (!ChunkMap.Contains(GetChunkKey(ChunkX, ChunkY, ChunkZ)))
					{
						LoadChunk(ChunkX, ChunkY, ChunkZ);
						return;
					}
				}
			}
		}
		
	}
}

void AVoxel_World::LoadChunk(int ChunkX, int ChunkY, int ChunkZ)
{
	if (HasAuthority())
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FString ChunkKey = GetChunkKey(ChunkX, ChunkY, ChunkZ);
		if (!ChunkMap.Contains(ChunkKey))
		{
			UE_LOG(LogTemp, Warning, TEXT("Load: %s"), *ChunkKey);
			ChunkMap.Add(ChunkKey, GetWorld()->SpawnActor<AVoxel_Chunk>(FVector(ChunkX * 3000, ChunkY * 3000, ChunkZ * 3000), FRotator(0, 0, 0), SpawnInfo));
			ChunkMap[ChunkKey]->Init(ChunkX, ChunkY, ChunkZ);
		}
	}
}

void AVoxel_World::UnloadChunk(int ChunkX, int ChunkY, int ChunkZ)
{
	if (HasAuthority())
	{
		FString ChunkKey = GetChunkKey(ChunkX, ChunkY, ChunkZ);
		if (ChunkMap.Contains(ChunkKey))
		{
			UE_LOG(LogTemp, Warning, TEXT("Destroy: %s"), *ChunkKey);
			//ChunkMap[ChunkKey]->SaveChunk();
			ChunkMap[ChunkKey]->Destroy();
			ChunkMap.Remove(ChunkKey);
		}
	}
}

uint16 AVoxel_World::GetBlock(int VoxelX, int VoxelY, int VoxelZ)
{
	int ChunkX = 0;
	int ChunkY = 0;
	int ChunkZ = 0;
	if (VoxelX < 0)
	{
		VoxelX++;
		ChunkX--;
	}
	if (VoxelY < 0)
	{
		VoxelY++;
		ChunkY--;
	}
	if (VoxelZ < 0)
	{
		VoxelZ++;
		ChunkZ--;
	}
	ChunkX = VoxelX / 30 + ChunkX;
	ChunkY = VoxelY / 30 + ChunkY;
	ChunkZ = VoxelZ / 30 + ChunkZ;

	FString ChunkKey = GetChunkKey(ChunkX, ChunkY, ChunkZ);

	if (ChunkMap.Contains(ChunkKey))
		return ChunkMap[ChunkKey]->GetBlock(VoxelX, VoxelY, VoxelZ);
	else
		return 0;
	
}

void AVoxel_World::SetBlock(int VoxelX, int VoxelY, int VoxelZ, int Id)
{
	if (HasAuthority())
	{
		int ChunkX = 0;
		int ChunkY = 0;
		int ChunkZ = 0;
		if (VoxelX < 0)
		{
			VoxelX++;
			ChunkX--;
		}
		if (VoxelY < 0)
		{
			VoxelY++;
			ChunkY--;
		}
		if (VoxelZ < 0)
		{
			VoxelZ++;
			ChunkZ--;
		}
		ChunkX = VoxelX / 30 + ChunkX;
		ChunkY = VoxelY / 30 + ChunkY;
		ChunkZ = VoxelZ / 30 + ChunkZ;

		FString ChunkKey = GetChunkKey(ChunkX, ChunkY, ChunkZ);
		if (ChunkMap.Contains(ChunkKey))
			ChunkMap[ChunkKey]->SetBlock(VoxelX, VoxelY, VoxelZ, Id);
	}
}

AVoxel_Chunk* AVoxel_World::GetChunk(int ChunkX, int ChunkY, int ChunkZ)
{
	FString ChunkKey = GetChunkKey(ChunkX, ChunkY, ChunkZ);

	if (ChunkMap.Contains(ChunkKey))
		return ChunkMap[ChunkKey];
	else
		return nullptr;
}