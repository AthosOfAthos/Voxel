// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Runnable.h"
#include "SkyfallenPillars.h"
#include "Rings.h"
#include "Beach.h"
#include "IsleofVines.h"
#include "Math/FastNoise.h"
#include "Biome.h"

class VOXEL_API Thread_Generation : public FRunnable
{
public:
	Thread_Generation(int WorldSeed);
	~Thread_Generation();

	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();

	bool Start(uint16* RefChunkData, int NewPosX, int NewPosY, int NewPosZ);
	void Confirm();

	bool IsActive = false;
	bool HasCompleted = true;
	int Seed;
	uint16* ChunkData;
	int PosX, PosY, PosZ;
};
