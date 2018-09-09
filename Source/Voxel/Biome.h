// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Math/FastNoise.h"
#include "SkyfallenPillars.h"
#include "Rings.h"
#include "CoreMinimal.h"

/**
 * 
 */
class VOXEL_API Biome
{
public:
	Biome(int WorldSeed);
	~Biome();
	FastNoise temperature;
	FastNoise rainfall;
	FastNoise islands;
	int GetBiome(int PosX, int PosY, int PosZ);
	int Seed;
};
