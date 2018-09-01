// Fill out your copyright notice in the Description page of Project Settings.

#include "SkyfallenPillars.h"
//This class generates the known and loved skyfallen pillar terrain
SkyfallenPillars::SkyfallenPillars()
{
	cellular = FastNoise();
	cellular.SetSeed(5);  
	cellular.SetFrequency(0.06); 
	cellular.SetCellularJitter(0.5);

	perlin = FastNoise();
	perlin.SetSeed(5);
	perlin.SetFrequency(0.07);
	folliage = 0.4;
}

SkyfallenPillars::~SkyfallenPillars()
{
}
/*
Very Important! Pass this method only the total position (voxel pos + chunk pos * 10
*/
int SkyfallenPillars::Generate(int VoxelX, int VoxelY, int VoxelZ) {
		int Value = Land(VoxelX, VoxelY, VoxelZ);
		if ((abs(perlin.GetPerlin(VoxelX*20, VoxelY*20, VoxelZ*20))>folliage)&&Value ==3) {
			Value = Detail(VoxelX, VoxelY, VoxelZ);
		}

		return Value;
}
int SkyfallenPillars::Land(int VoxelX, int VoxelY, int VoxelZ) {
	float Mheight = cellular.GetCellular((VoxelX) * 1, (VoxelY) * 1);
	Mheight *= 30;

	if (perlin.GetPerlinFractal(VoxelX, VoxelY, VoxelZ) <= 0.015) {
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
