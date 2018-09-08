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
						switch (GetBiome(PosX / 30, PosY / 30, PosZ / 30)) {
						case 0:
							ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)] = pillars.Generate(VoxelX + (PosX * 30), VoxelY + (PosY * 30), VoxelZ + (PosZ * 30));
							break;
						case 1:
							ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)] = pillars.Generate(VoxelX + (PosX * 30), VoxelY + (PosY * 30), VoxelZ + (PosZ * 30));
							break;
						default:
							ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)] = 0;
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
	float temp = temperature.GetPerlin(PosX, PosY);
	float rain = rainfall.GetPerlin(PosX, PosY);
	if (temp >= rain)
		return 1;
	else
		return 1;

	return 1;
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