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
		//Right here we set all the perlin noise variables 
		perlin = FastNoise();
		perlin.SetSeed(5); //Controls the world seed. Should be randomized 
		perlin.SetFrequency(0.01);// Controls frequency, larger numbers mean more smaller holes, 
		perlin.SetFractalGain(0.5); //Controls Gain, probably is persistance, keep near 0.5,
		perlin.SetFractalLacunarity(5); //Controls Lacunarity, messes with seperation higher less, 
		perlin.SetFractalOctaves(7); //Controls Octaves, controls detail , Base 3 Prefered control
		perlin.SetCellularJitter(0.5); //um bigger better
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(perlin.GetFrequency()));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(perlin.GetFractalGain()));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(perlin.GetFractalLacunarity()));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(perlin.GetFractalOctaves()));
		

	}
}

// Called every frame
void AVoxel_World::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		PlayerLocations[0].X += DeltaTime * 1;
		//TODO: dont do this every tick
		ManageChunks();
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
	for (int i = 0; i < PlayerLocations.Num(); i++)
	{
		FVector PlayerChunk = FVector(PlayerLocations[i].X / 10, PlayerLocations[i].Y / 10, PlayerLocations[i].Z / 10);
		for (int ChunkX = PlayerChunk.X - ViewRadius; ChunkX < PlayerChunk.X + ViewRadius; ChunkX++)
		{
			for (int ChunkY = PlayerChunk.Y - ViewRadius; ChunkY < PlayerChunk.Y + ViewRadius; ChunkY++)
			{
				for (int ChunkZ = PlayerChunk.Z - ViewRadius; ChunkZ < PlayerChunk.Z + ViewRadius; ChunkZ++)
				{
					FString ChunkKey = GetChunkKey(ChunkX, ChunkY, ChunkZ);
					if (!ChunkMap.Contains(ChunkKey))
						LoadChunk(ChunkX,ChunkY, ChunkZ);
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
			ChunkMap.Add(ChunkKey, GetWorld()->SpawnActor<AVoxel_Chunk>(FVector(ChunkX * 1000, ChunkY * 1000, ChunkZ * 1000), FRotator(0, 0, 0), SpawnInfo));
			ChunkMap[ChunkKey]->Init(ChunkX, ChunkY, ChunkZ, &perlin);
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
			ChunkMap[ChunkKey]->SaveChunk();
			ChunkMap[ChunkKey]->Destroy();
			ChunkMap.Remove(ChunkKey);
		}
	}
}

uint16 AVoxel_World::GetBlock(int VoxelX, int VoxelY, int VoxelZ)
{
	FString ChunkKey = GetChunkKey(VoxelX / 10, VoxelY / 10, VoxelZ / 10);

	return ChunkMap[ChunkKey]->GetBlock(VoxelX, VoxelY, VoxelZ);
}

void AVoxel_World::SetBlock(int VoxelX, int VoxelY, int VoxelZ, int Id)
{
	if (HasAuthority())
	{
		FString ChunkKey = GetChunkKey(VoxelX / 10, VoxelY / 10, VoxelZ / 10);

		ChunkMap[ChunkKey]->SetBlock(VoxelX, VoxelY, VoxelZ, Id);
	}
}
