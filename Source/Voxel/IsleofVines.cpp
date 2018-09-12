// Fill out your copyright notice in the Description page of Project Settings.

#include "IsleofVines.h"
//This class generates the isle terrain
IsleofVines::IsleofVines(int Seed)
{
	land = FastNoise();
	land.SetSeed(Seed);
	land.SetFrequency(0.02);
	land.SetFractalOctaves(2);

	overhangs = FastNoise();
	overhangs.SetSeed(Seed);
	overhangs.SetFrequency(0.05);
	overhangs.SetFractalGain(0.1);

	isle = FastNoise();
	isle.SetSeed(Seed);
	isle.SetFrequency(0.005);
	isle.SetCellularJitter(1);
	folliage = 0.4;
}

IsleofVines::~IsleofVines()
{
}
/*
Very Important! Pass this method only the total position (voxel pos + chunk pos * 10
*/
int IsleofVines::Generate(int VoxelX, int VoxelY, int VoxelZ) {
	int Value = Land(VoxelX, VoxelY, VoxelZ);

	//Value = Detail(VoxelX, VoxelY, VoxelZ, Value);


	return Value;
}
int IsleofVines::Land(int VoxelX, int VoxelY, int VoxelZ) {
	int Value = 0;
	if (VoxelZ < -15) {//Controls floor stop
		return 0;
	}

	float Mheight = land.GetPerlinFractal(VoxelX, VoxelY);
	float Rock = land.GetCubic(VoxelX, VoxelY);
	float is = isle.GetCellular(VoxelX, VoxelY);
	float o = overhangs.GetPerlinFractal(VoxelX, VoxelY, VoxelZ);
	float o2 = overhangs.GetPerlinFractal(VoxelX, VoxelY, VoxelZ+1);
	float o3 = overhangs.GetPerlinFractal(VoxelX, VoxelY, VoxelZ+2);
	is *= 50;
	Rock = abs(Rock)*60;
	Mheight = Mheight * 60+(30);
	if (VoxelZ < Mheight - 2 && VoxelZ >= Rock && VoxelZ < is - 2) {
			Value = 101;
	}
	else if (VoxelZ < Mheight - 1 && VoxelZ >= Rock && VoxelZ < is - 1) {
			Value = 130;
	}
	else if (VoxelZ < Mheight && VoxelZ >= Rock && VoxelZ < is) {
			Value = 131;
	}
	float cut = -0.3;
	if (Value != 0) {
		if (o > cut) {
			Value = 0;
		}
	}
	if (Value == 101) {
		if (o2 > cut) {
			Value = 131;//131
		}
		else if (o3 > cut) {
			Value = 130;
		}
	}
	return Value;
}
int IsleofVines::Detail(int VoxelX, int VoxelY, int VoxelZ, int Value) {
	if ((abs(overhangs.GetPerlin(VoxelX * 20, VoxelY * 20, VoxelZ * 20)) > folliage) && Value == 3) {
		return 132;
	}
	return Value; //So the tricky part here is it can only pass 1 block and needs to place like a tree or something, which could be algebraic trees or be grown
}

