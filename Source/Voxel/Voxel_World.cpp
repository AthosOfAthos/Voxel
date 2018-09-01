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

// Called when the game starts or when spawned
void AVoxel_World::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		 
		cellular = FastNoise();
		cellular.SetSeed(5); //Controls the world seed. Should be randomized 
		cellular.SetFrequency(0.05);// Controls frequency, larger numbers mean more smaller holes, 
		cellular.SetCellularJitter(0.5);//randomness

		//preload world
		for (int ChunkX = -2; ChunkX < 2; ChunkX++)
		{
			for (int ChunkY = -2; ChunkY < 2; ChunkY++)
			{
				for (int ChunkZ = -2; ChunkZ < 2; ChunkZ++)
				{
					LoadChunk(ChunkX, ChunkY, ChunkZ);
				}
			}
		}
		PreloadComplete = true;

		GetWorld()->GetTimerManager().SetTimer(ChunkTimer, this, &AVoxel_World::ManageChunks, 0.1, true, 0);
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
		for (int i = 0; i < PlayerLocations.Num(); i++)
		{
			//Correct world space to chunk space
			FVector PlayerChunk = FVector(PlayerLocations[i].X / 1000, PlayerLocations[i].Y / 1000, PlayerLocations[i].Z / 1000);

			//Quick load near chunks
			for (int ChunkX = PlayerChunk.X - 3; ChunkX < PlayerChunk.X + 3; ChunkX++)
			{
				for (int ChunkY = PlayerChunk.Y - 3; ChunkY < PlayerChunk.Y + 3; ChunkY++)
				{
					for (int ChunkZ = PlayerChunk.Z - 3; ChunkZ < PlayerChunk.Z + 3; ChunkZ++)
					{
						FString ChunkKey = GetChunkKey(ChunkX, ChunkY, ChunkZ);
						if (!ChunkMap.Contains(ChunkKey))
						{
							LoadChunk(ChunkX, ChunkY, ChunkZ);
						}
					}
				}
			}
		}
		
		//ManageCleanup();

		for (int i = 0; i < PlayerLocations.Num(); i++)
		{
			//Correct world space to chunk space
			FVector PlayerChunk = FVector(PlayerLocations[i].X / 1000, PlayerLocations[i].Y / 1000, PlayerLocations[i].Z / 1000);

			for (int ChunkX = PlayerChunk.X - ViewRadius; ChunkX < PlayerChunk.X + ViewRadius; ChunkX++)
			{
				for (int ChunkY = PlayerChunk.Y - ViewRadius; ChunkY < PlayerChunk.Y + ViewRadius; ChunkY++)
				{
					for (int ChunkZ = PlayerChunk.Z - ViewRadius; ChunkZ < PlayerChunk.Z + ViewRadius; ChunkZ++)
					{
						FString ChunkKey = GetChunkKey(ChunkX, ChunkY, ChunkZ);
						if (!ChunkMap.Contains(ChunkKey))
						{
							LoadChunk(ChunkX, ChunkY, ChunkZ);
							return;
						}
					}
				}
			}
		}



	}
}

void AVoxel_World::ManageCleanup()
{
	if (HasAuthority())
	{
		//Clean far Chunks
		int MaxX = 0;
		int MaxY = 0;
		int MaxZ = 0;
		TArray<AVoxel_Chunk*> LoadedChunks;
		ChunkMap.GenerateValueArray(LoadedChunks);
		for (int i = 0; i < LoadedChunks.Num(); i++)
		{
			for (int p = 0; p < PlayerLocations.Num(); p++)
			{
				FVector PlayerChunk = FVector(PlayerLocations[p].X / 1000, PlayerLocations[p].Y / 1000, PlayerLocations[p].Z / 1000);

				if (FMath::Abs(LoadedChunks[i]->PosX - PlayerChunk.X) > MaxX)
					MaxX = FMath::Abs(LoadedChunks[i]->PosX - PlayerChunk.X);
				if (FMath::Abs(LoadedChunks[i]->PosY - PlayerChunk.Y) > MaxY)
					MaxY = FMath::Abs(LoadedChunks[i]->PosY - PlayerChunk.Y);
				if (FMath::Abs(LoadedChunks[i]->PosZ - PlayerChunk.Z) > MaxZ)
					MaxZ = FMath::Abs(LoadedChunks[i]->PosZ - PlayerChunk.Z);
			}

			if (FMath::Abs(MaxX) > ViewRadius + 1 || FMath::Abs(MaxY) > ViewRadius + 1 || FMath::Abs(MaxZ) > ViewRadius + 1)
			{
				UnloadChunk(LoadedChunks[i]->PosX, LoadedChunks[i]->PosY, LoadedChunks[i]->PosZ);
				return;
			}

			//UE_LOG(LogTemp, Warning, TEXT("Distance: %s"), *FVector(MaxX, MaxY, MaxZ).ToString());
			MaxX = 0;
			MaxY = 0;
			MaxZ = 0;
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
			ChunkMap.Add(ChunkKey, GetWorld()->SpawnActor<AVoxel_Chunk>(FVector(ChunkX * 1000, ChunkY * 1000, ChunkZ * 1000), FRotator(0, 0, 0), SpawnInfo));
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
			ChunkMap[ChunkKey]->SaveChunk();
			ChunkMap[ChunkKey]->Destroy();
			ChunkMap.Remove(ChunkKey);
		}
	}
}

uint16 AVoxel_World::GetBlock(int VoxelX, int VoxelY, int VoxelZ)
{
	FString ChunkKey = GetChunkKey(VoxelX / 10, VoxelY / 10, VoxelZ / 10);

	if (ChunkMap.Contains(ChunkKey))
		return ChunkMap[ChunkKey]->GetBlock(VoxelX, VoxelY, VoxelZ);
	else
		return 0;
	
}

void AVoxel_World::SetBlock(int VoxelX, int VoxelY, int VoxelZ, int Id)
{
	if (HasAuthority())
	{
		FString ChunkKey = GetChunkKey(VoxelX / 10, VoxelY / 10, VoxelZ / 10);

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