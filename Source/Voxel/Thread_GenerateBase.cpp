// Fill out your copyright notice in the Description page of Project Settings.

#include "Thread_GenerateBase.h"

Thread_GenerateBase::Thread_GenerateBase()
{
	
}

Thread_GenerateBase::~Thread_GenerateBase()
{

}

void Thread_GenerateBase::Setup(int NewX, int NewY, int NewZ, uint16* ChunkRef)
{
	Thread_GenerateBase::PosX = NewX;
	Thread_GenerateBase::PosY = NewY;
	Thread_GenerateBase::PosZ = NewZ;
	Thread_GenerateBase::ChunkData = ChunkRef;
}

bool Thread_GenerateBase::Init()
{
	return true;
}

uint32 Thread_GenerateBase::Run()
{
	for (int8 VoxelX = 0; VoxelX < 10; VoxelX++)
	{
		for (int8 VoxelY = 0; VoxelY < 10; VoxelY++)
		{
			for (int8 VoxelZ = 0; VoxelZ < 10; VoxelZ++)
			{
				ChunkData[VoxelX + (VoxelY * 10) + (VoxelZ * 100)] = skp.Generate(VoxelX + (PosX * 10), VoxelY + (PosY * 10), VoxelZ + (PosZ * 10));
			}
		}
	}

	IsFinished = true;
	return 0;
}

void Thread_GenerateBase::Stop()
{

}