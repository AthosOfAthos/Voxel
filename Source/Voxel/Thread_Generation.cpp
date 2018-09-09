// Fill out your copyright notice in the Description page of Project Settings.

#include "Thread_Generation.h"

Thread_Generation::Thread_Generation(int WorldSeed)
{
	Seed = WorldSeed;

	
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
	Biome biome = Biome(Seed);
	Beach beach = Beach(Seed);
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
						switch (biome.GetBiome(PosX, PosY, PosZ)) {//If I wanted! I could also curve based on smaller detail with Voxel
						case 0:
							ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)] = 0;
							break;
						case 1:
							ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)] = rings.Generate(VoxelX + (PosX * 30), VoxelY + (PosY * 30), VoxelZ + (PosZ * 30));
							break;
						case 2:
							ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)] = pillars.Generate(VoxelX + (PosX * 30), VoxelY + (PosY * 30), VoxelZ + (PosZ * 30));
							break;
						case 3:
							ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)] = beach.Generate(VoxelX + (PosX * 30), VoxelY + (PosY * 30), VoxelZ + (PosZ * 30));
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