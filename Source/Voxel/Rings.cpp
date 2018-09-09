// Fill out your copyright notice in the Description page of Project Settings.

#include "Rings.h"
#include "Voxel_World.h"
#include "VoxelGameModeBase.h"
//This class generates something
Rings::Rings(int Seed)
{
	land = FastNoise();
	land.SetSeed(Seed);
	land.SetFrequency(0.01);
	land.SetFractalOctaves(5);

	overhangs = FastNoise();
	overhangs.SetSeed(Seed);
	overhangs.SetFrequency(0.1);
	overhangs.SetFractalGain(0.1);

	rings = FastNoise();
	rings.SetSeed(Seed);
	rings.SetFrequency(0.01);
	rings.SetFractalLacunarity(2);
	rings.SetFractalOctaves(5);
	folliage = 0.4;
}

Rings::~Rings()
{
}
/*
Very Important! Pass this method only the total position (voxel pos + chunk pos * 10
*/
int Rings::Generate(int VoxelX, int VoxelY, int VoxelZ) {
	int Value = Land(VoxelX, VoxelY, VoxelZ);

	//Value = Detail(VoxelX, VoxelY, VoxelZ, Value);


	return Value;
}
int Rings::Land(int VoxelX, int VoxelY, int VoxelZ) {
	int Value = 0;
	if (VoxelZ < -10) {//Controls floor stop
		return 0;
	}
	//Right here I'm going to draw a large map of how I want islands to work
	if (VoxelZ == 1) {
		if (land.GetPerlin(VoxelX, VoxelY) > 0) {
			Value = 101;
		}
		else if (land.GetPerlin(VoxelX, VoxelY) < 0) {
			Value = 130;
		}

	}
	return Value;
}
int Rings::Detail(int VoxelX, int VoxelY, int VoxelZ, int Value) {
	if ((abs(overhangs.GetPerlin(VoxelX * 20, VoxelY * 20, VoxelZ * 20)) > folliage)) {
		
	}
	return Value; //So the tricky part here is it can only pass 1 block and needs to place like a tree or something, which could be algebraic trees or be grown
}
