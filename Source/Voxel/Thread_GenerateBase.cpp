// Fill out your copyright notice in the Description page of Project Settings.

#include "Thread_GenerateBase.h"

Thread_GenerateBase::Thread_GenerateBase()
{
	temperature = FastNoise();
	temperature.SetSeed(5);
	temperature.SetFrequency(0.01);

	rainfall = FastNoise();
	rainfall.SetSeed(6);
	rainfall.SetFrequency(0.01);
}

Thread_GenerateBase::~Thread_GenerateBase()
{

}

void Thread_GenerateBase::Setup(int NewX, int NewY, int NewZ, uint16* RefChunkData)
{
	Thread_GenerateBase::PosX = NewX;
	Thread_GenerateBase::PosY = NewY;
	Thread_GenerateBase::PosZ = NewZ;
	Thread_GenerateBase::ChunkData = RefChunkData;
}

bool Thread_GenerateBase::Init()
{
	return true;
}

uint32 Thread_GenerateBase::Run()
{
	for (int8 VoxelX = 0; VoxelX < 30; VoxelX++)
	{
		for (int8 VoxelY = 0; VoxelY < 30; VoxelY++)
		{
			for (int8 VoxelZ = 0; VoxelZ < 30; VoxelZ++)
			{
				switch (GetBiome(PosX/30, PosY/30, PosZ/30)) {
					case 0:
						ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)] = pillars.Generate(VoxelX + (PosX * 30), VoxelY + (PosY * 30), VoxelZ + (PosZ * 30));
						break;
					case 1:
						ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)] = rings.Generate(VoxelX + (PosX * 30), VoxelY + (PosY * 30), VoxelZ + (PosZ * 30));
						break;
					default:
						ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)] = 0;
				}
				
				//ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)] = 0;
				//ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)] = 100;
			}
		}
	}

	IsFinished = true;
	return 0;
}

int Thread_GenerateBase::GetBiome(int PosX, int PosY, int PosZ) {
	float temp = temperature.GetPerlin(PosX, PosY);
	float rain = rainfall.GetPerlin(PosX, PosY);
	if (temp >= rain) {
		return 1;
	}
	else {
		return 1;
	}
	return 1;
	//Current plan! I will have a matrix of sorts for biomes based on temp and rainfall (
}

void Thread_GenerateBase::Stop()
{

}