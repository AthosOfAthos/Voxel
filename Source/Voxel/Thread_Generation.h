// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Runnable.h"
#include "Pillars.h"
#include "Rings.h"
#include "Math/FastNoise.h"

class VOXEL_API Thread_Generation : public FRunnable
{
public:
	Thread_Generation(int WorldSeed);
	~Thread_Generation();

	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();

	int GetBiome(int PosX, int PosY, int PosZ);

	void Start(uint16* RefChunkData, int NewPosX, int NewPosY, int NewPosZ);
	void Confirm();

	FastNoise temperature;
	FastNoise rainfall;

	bool IsActive = false;
	bool HasCompleted = true;
	int Seed;
	uint16* ChunkData;
	int PosX, PosY, PosZ;
};
