// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "UnrealNetwork.h"
#include "Math/FastNoise.h"
#include "CoreMinimal.h"

/**
 *
 */
class VOXEL_API Beach
{
public:
	Beach(int);
	~Beach();

	int Generate(int VoxelX, int VoxelY, int VoxelZ);
	int Land(int VoxelX, int VoxelY, int VoxelZ);
	int Detail(int VoxelX, int VoxelY, int VoxelZ, int Value);
	FastNoise overhangs;
	FastNoise land;

	double folliage;
};
