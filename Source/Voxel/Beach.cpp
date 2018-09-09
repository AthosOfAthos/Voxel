// Fill out your copyright notice in the Description page of Project Settings.

#include "Beach.h"
//This class generates the pillars terrain
Beach::Beach(int Seed)
{
	land = FastNoise();
	land.SetSeed(Seed);
	land.SetFrequency(0.01);
	land.SetFractalOctaves(5);

	overhangs = FastNoise();
	overhangs.SetSeed(Seed);
	overhangs.SetFrequency(0.1);
	overhangs.SetFractalGain(0.1);

	folliage = 0.4;
}

Beach::~Beach()
{
}
/*
Very Important! Pass this method only the total position (voxel pos + chunk pos * 10
*/
int Beach::Generate(int VoxelX, int VoxelY, int VoxelZ) {
	int Value = Land(VoxelX, VoxelY, VoxelZ);

	//Value = Detail(VoxelX, VoxelY, VoxelZ, Value);


	return Value;
}
int Beach::Land(int VoxelX, int VoxelY, int VoxelZ) {
	int Value = 0;
	if (VoxelZ < -10) {//Controls floor stop
		return 0;
	}

	float Mheight = land.GetPerlinFractal((VoxelX) * 1, (VoxelY) * 1);

	Mheight *= 10;
	if (VoxelZ < Mheight) {
		return 132;
	}
	return Value;
}
int Beach::Detail(int VoxelX, int VoxelY, int VoxelZ, int Value) {
	if ((abs(overhangs.GetPerlin(VoxelX * 20, VoxelY * 20, VoxelZ * 20)) > folliage) && Value == 3) {
		return 132;
	}
	return Value; //So the tricky part here is it can only pass 1 block and needs to place like a tree or something, which could be algebraic trees or be grown
}
