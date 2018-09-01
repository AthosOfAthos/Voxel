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
	UE_LOG(LogTemp, Warning, TEXT("Thread Start!"));
	FPlatformProcess::Sleep(1);
	
	for (int8 VoxelX = 0; VoxelX < 10; VoxelX++)
	{
		for (int8 VoxelY = 0; VoxelY < 10; VoxelY++)
		{
			for (int8 VoxelZ = 0; VoxelZ < 10; VoxelZ++)
			{
				
				float Height = Noise.GetPerlinFractal((VoxelX + (PosX * 10)) * 1, (VoxelY + (PosY * 10)) * 1);
				Height *= 20;
				if ((VoxelZ + (PosZ * 10)) < (Height - 1))
				{
					ChunkData[VoxelX + (VoxelY * 10) + (VoxelZ * 100)] = 2;
				}
				else if ((VoxelZ + (PosZ * 10)) < Height) {
					ChunkData[VoxelX + (VoxelY * 10) + (VoxelZ * 100)] = 3;
				}
				else
				{
					ChunkData[VoxelX + (VoxelY * 10) + (VoxelZ * 100)] = 0;
				}

			}
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Thread End!"));
	IsFinished = true;
	return 0;
}

void Thread_GenerateBase::Stop()
{

}