// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxel_World.h"


// Sets default values
AVoxel_World::AVoxel_World()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

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

		//Spawn 0,0,0 for testing
		
		//ChunkMap.Add(TEXT("0,0,0"), GetWorld()->SpawnActor<AVoxel_Chunk>(FVector(0, 0, 0), FRotator(0, 0, 0), SpawnInfo));
		//ChunkMap[TEXT("0,0,0")]->Init(0, 0, 0, &perlin);
		

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


		GetWorld()->SpawnActor<AVoxel_Chunk>(FVector(0, 0, 0), FRotator(0, 0, 0), SpawnInfo);

		for (int ChunkX = 0; ChunkX < 3; ChunkX++)
		{
			for (int ChunkY = 0; ChunkY < 3; ChunkY++)
			{
				for (int ChunkZ = 0; ChunkZ < 3; ChunkZ++)
				{
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
		}






	}

	
	

}

// Called every frame
void AVoxel_World::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

