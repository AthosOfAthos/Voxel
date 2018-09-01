// Fill out your copyright notice in the Description page of Project Settings.

#include "SkyfallenPillars.h"
//This class generates the known and loved skyfallen pillar terrain
SkyfallenPillars::SkyfallenPillars()
{
	cellular = FastNoise();
	cellular.SetSeed(5);  
	cellular.SetFrequency(0.05); 
	cellular.SetCellularJitter(0.5);

	perlin = FastNoise();
	perlin.SetSeed(5);
	perlin.SetFrequency(0.05);
	folliageDensity = 0.5;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(folliageDensity));
}

SkyfallenPillars::~SkyfallenPillars()
{
}
/*
Very Important! Pass this method only the total position (voxel pos + chunk pos * 10
*/
int SkyfallenPillars::Generate(int VoxelX, int VoxelY, int VoxelZ) {
		int Value = Land(VoxelX, VoxelY, VoxelZ);
		if ((abs(perlin.GetPerlin(VoxelX, VoxelY, VoxelZ))>folliageDensity)&&Value ==3) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(abs(perlin.GetPerlin(VoxelX, VoxelY, VoxelZ))));
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(folliageDensity));
			Value = Detail(VoxelX, VoxelY, VoxelZ);
		}

		return Value;
}
int SkyfallenPillars::Land(int VoxelX, int VoxelY, int VoxelZ) {
	float Mheight = cellular.GetCellular((VoxelX) * 1, (VoxelY) * 1);
	Mheight *= 30;

	if (perlin.GetPerlinFractal(VoxelX, VoxelY, VoxelZ) <= 0.01) {
		if (VoxelZ < (Mheight - 1)) {
			return 2;

		}
		else if (VoxelZ <= (Mheight)) {
			return 3;
		}

	}
	return 0;
}
int SkyfallenPillars::Detail(int VoxelX, int VoxelY, int VoxelZ) {
	return 4; //So the tricky part here is it can only pass 1 block and needs to place like a tree or something, which could be algebraic trees or be grown
}
