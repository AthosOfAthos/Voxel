// Fill out your copyright notice in the Description page of Project Settings.

#include "Thread_GenerateBase.h"

Thread_GenerateBase::Thread_GenerateBase()
{

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
				ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)] = pillars.Generate(VoxelX + (PosX * 30), VoxelY + (PosY * 30), VoxelZ + (PosZ * 30));
			}
		}
	}

	IsFinished = true;
	return 0;
}

void Thread_GenerateBase::Stop()
{

}