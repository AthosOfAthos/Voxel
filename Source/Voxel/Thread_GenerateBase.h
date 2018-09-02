// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Runnable.h"
#include "SkyfallenPillars.h"
#include "Math/FastNoise.h"

class Thread_GenerateBase : public FRunnable
{
public:
	Thread_GenerateBase();
	virtual ~Thread_GenerateBase();

	void Setup(int NewX, int NewY, int NewZ, uint16* RefChunkData);

	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();

	uint16* ChunkData;

	bool IsFinished = false;

	SkyfallenPillars skp = SkyfallenPillars();

	int PosX, PosY, PosZ;
};
