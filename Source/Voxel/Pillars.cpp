// Fill out your copyright notice in the Description page of Project Settings.

#include "Pillars.h"
//This class generates the known and loved skyfallen pillar terrain
Pillars::Pillars()
{
	land = FastNoise();
	land.SetSeed(5);
	land.SetFrequency(0.01);
	land.SetFractalOctaves(5);

	overhangs = FastNoise();
	overhangs.SetSeed(5);
	overhangs.SetFrequency(0.1);
	overhangs.SetFractalGain(0.1);

	pillars = FastNoise();
	pillars.SetSeed(5);
	pillars.SetFrequency(0.05);
	pillars.SetCellularJitter(0.9);
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

	//Value = Detail(VoxelX, VoxelY, VoxelZ, Value);


	return Value;
}
int Pillars::Land(int VoxelX, int VoxelY, int VoxelZ) {
	if (VoxelZ < -50) {//Controls floor stop
		return 2;
	}

	float Mheight = land.GetPerlinFractal((VoxelX) * 1, (VoxelY) * 1);
	Mheight *= 20;
	float pillar = pillars.GetCellular((VoxelX) * 1, (VoxelY) * 1);

	if (pillar > 0.5) {
		pillar *= 50;
	}
	else {
		pillar = 0;
	}

	if (VoxelZ > Mheight && pillar != 0) {
		if (overhangs.GetPerlinFractal(VoxelX, VoxelY, VoxelZ) <= 0.1) { //Pillar cuts
			if (VoxelZ < (pillar - 2)) {
				return 2;

			}
			else if (VoxelZ <= (pillar)) {
				return 3;
			}
		}
	}else if (overhangs.GetPerlinFractal(VoxelX, VoxelY, VoxelZ) <= 0.25) {//Land cave size
		if (VoxelZ < (Mheight - 2)) {
			return 2;

		}
		else if (VoxelZ <= (Mheight)) {
			return 3;
		}

	}
	return 0;
}
int Pillars::Detail(int VoxelX, int VoxelY, int VoxelZ, int Value) {
	if ((abs(overhangs.GetPerlin(VoxelX * 20, VoxelY * 20, VoxelZ * 20)) > folliage) && Value == 3) {
		return 4;
	}
	return Value; //So the tricky part here is it can only pass 1 block and needs to place like a tree or something, which could be algebraic trees or be grown
}
