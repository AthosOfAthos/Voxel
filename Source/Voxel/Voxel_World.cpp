// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxel_World.h"


// Sets default values
AVoxel_World::AVoxel_World()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	//Load Generic Voxel mesh
	BlockMesh = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Cube.Cube"));

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
		

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		for (int ChunkX = -5; ChunkX < 5; ChunkX++)
		{
			for (int ChunkY = -5; ChunkY < 5; ChunkY++)

			{
				for (int ChunkZ = -2; ChunkZ < 2; ChunkZ++)
				{
					LoadChunk(ChunkX, ChunkY, ChunkZ);
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

void AVoxel_World::LoadChunk(int ChunkX, int ChunkY, int ChunkZ)
{
	if (HasAuthority())
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FString ChunkKey = FString().SanitizeFloat(ChunkX);
		ChunkKey.Append(",");
		ChunkKey.Append(FString().SanitizeFloat(ChunkY));
		ChunkKey.Append(",");
		ChunkKey.Append(FString().SanitizeFloat(ChunkZ));
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, ChunkKey);
		ChunkMap.Add(ChunkKey, GetWorld()->SpawnActor<AVoxel_Chunk>(FVector(ChunkX * 1000, ChunkY * 1000, ChunkZ * 1000), FRotator(0, 0, 0), SpawnInfo));
		ChunkMap[ChunkKey]->Init(ChunkX, ChunkY, ChunkZ, &perlin);
	}
}

void AVoxel_World::UnloadChunk(int ChunkX, int ChunkY, int ChunkZ)
{
	if (HasAuthority())
	{
		FString ChunkKey = FString().SanitizeFloat(ChunkX);
		ChunkKey.Append(",");
		ChunkKey.Append(FString().SanitizeFloat(ChunkY));
		ChunkKey.Append(",");
		ChunkKey.Append(FString().SanitizeFloat(ChunkZ));
		ChunkMap[ChunkKey]->Destroy();
		ChunkMap.Remove(ChunkKey);
	}
}

uint16 AVoxel_World::GetBlock(int VoxelX, int VoxelY, int VoxelZ)
{
	FString ChunkKey = FString().SanitizeFloat(VoxelX / 10);
	ChunkKey.Append(",");
	ChunkKey.Append(FString().SanitizeFloat(VoxelY / 10));
	ChunkKey.Append(",");
	ChunkKey.Append(FString().SanitizeFloat(VoxelZ / 10));

	return ChunkMap[ChunkKey]->GetBlock(VoxelX, VoxelY, VoxelZ);
}

void AVoxel_World::SetBlock(int VoxelX, int VoxelY, int VoxelZ, int Id)
{
	if (HasAuthority())
	{
		FString ChunkKey = FString().SanitizeFloat(VoxelX / 10);
		ChunkKey.Append(",");
		ChunkKey.Append(FString().SanitizeFloat(VoxelY / 10));
		ChunkKey.Append(",");
		ChunkKey.Append(FString().SanitizeFloat(VoxelZ / 10));

		ChunkMap[ChunkKey]->SetBlock(VoxelX, VoxelY, VoxelZ, Id);
	}
}
