// Fill out your copyright notice in the Description page of Project Settings.

#include "Thread_Generation.h"

Thread_Generation::Thread_Generation(int WorldSeed)
{
	Seed = WorldSeed;

	temperature = FastNoise();
	temperature.SetSeed(Seed);
	temperature.SetFrequency(0.01);

	rainfall = FastNoise();
	rainfall.SetSeed(Seed + 1);
	rainfall.SetFrequency(0.01);

	islands = FastNoise();
	islands.SetSeed(Seed+2);
	islands.SetFrequency(0.01);
}

Thread_Generation::~Thread_Generation()
{
}

bool Thread_Generation::Init()
{
	return true;
}

uint32 Thread_Generation::Run()
{
	Pillars pillars = Pillars(Seed);
	Rings rings = Rings(Seed);

	while (true)
	{
		if (IsActive && !HasCompleted)
		{
			//Thread is active and currently generating terrain

			//START// generation code
			for (int8 VoxelX = 0; VoxelX < 30; VoxelX++)
			{
				for (int8 VoxelY = 0; VoxelY < 30; VoxelY++)
				{
					for (int8 VoxelZ = 0; VoxelZ < 30; VoxelZ++)
					{
						int B = GetBiome(PosX / 30, PosY / 30, PosZ / 30);
						switch (B) {
						case 0:

							ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)] = 0;
							break;
						case 1:
							ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)] = rings.Generate(VoxelX + (PosX * 30), VoxelY + (PosY * 30), VoxelZ + (PosZ * 30));
							break;
						case 2:
							ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)] = pillars.Generate(VoxelX + (PosX * 30), VoxelY + (PosY * 30), VoxelZ + (PosZ * 30));
							break;
						default:
							ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)] = 0;
							break;
						}

					}
				}
			}
			//END// generation code

			HasCompleted = true;
		}
		else
		{
			//Thread awaiting orders
			FPlatformProcess::Sleep(0.05);
		}
	}

	return 0;
}

void Thread_Generation::Stop()
{

}

int Thread_Generation::GetBiome(int PosX, int PosY, int PosZ)
{
	//First I check if it is part of an island
	if (PosX == 0 && PosY == 0 && PosZ == 0) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(islands.GetPerlin((PosX) * 1, (PosY) * 1) > 0));
	}
	if (islands.GetPerlin((PosX) * 1, (PosY) * 1)>0||true) {
		//Then I check which biome it is
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "island");
		if (rainfall.GetPerlin((PosX)*10, (PosY)*10)<temperature.GetPerlin((PosX)*1, (PosY)*1)) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "1");
			return 1;
		}
		else {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "2");
			return 2;
		}
	}
	else {
		return 0;
	}
	//Current plan! I will have a matrix of sorts for biomes based on temp and rainfall (
}

void Thread_Generation::Start(uint16* RefChunkData, int NewPosX, int NewPosY, int NewPosZ)
{
	if (!IsActive)
	{
		ChunkData = RefChunkData;
		PosX = NewPosX;
		PosY = NewPosY;
		PosZ = NewPosZ;

		IsActive = true;
		HasCompleted = false;
	}
}

void Thread_Generation::Confirm()
{
	IsActive = false;
}