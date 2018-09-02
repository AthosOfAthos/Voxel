// Fill out your copyright notice in the Description page of Project Settings.

#include "Pillars.h"
//This class generates the known and loved skyfallen pillar terrain
Pillars::Pillars()
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

Pillars::~Pillars()
{
}
/*
Very Important! Pass this method only the total position (voxel pos + chunk pos * 10
*/
int Pillars::Generate(int VoxelX, int VoxelY, int VoxelZ) {
		int Value = Land(VoxelX, VoxelY, VoxelZ);
		
		Value = Detail(VoxelX, VoxelY, VoxelZ, Value);
	

		return Value;
}
int Pillars::Land(int VoxelX, int VoxelY, int VoxelZ) {
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
int Pillars::Detail(int VoxelX, int VoxelY, int VoxelZ, int Value) {
	if ((abs(perlin.GetPerlin(VoxelX * 20, VoxelY * 20, VoxelZ * 20)) > folliage) && Value == 3) {
		return 4;
	}
	return Value; //So the tricky part here is it can only pass 1 block and needs to place like a tree or something, which could be algebraic trees or be grown
}
