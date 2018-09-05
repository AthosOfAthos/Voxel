// Fill out your copyright notice in the Description page of Project Settings.

#include "Rings.h"
//This class generates the known and loved skyfallen ring terrain
Rings::Rings()
{
	land = FastNoise();
	land.SetSeed(6);
	land.SetFrequency(0.01);
	land.SetFractalOctaves(5);

	overhangs = FastNoise();
	overhangs.SetSeed(6);
	overhangs.SetFrequency(0.1);
	overhangs.SetFractalGain(0.1);

	rings = FastNoise();
	rings.SetSeed(6);
	rings.SetFrequency(0.005);
	rings.SetFractalLacunarity(2);
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
	if (VoxelZ < -0) {//Controls floor stop
		return 0;
	}

	float Mheight = land.GetPerlin((VoxelX) * 1, (VoxelY) * 1);

	float ring = rings.GetCubic((VoxelX) * 1, (VoxelY) * 5);//Considering messing with X/Y ratio to create arches
	float r2 = rings.GetCubic((VoxelX) * 5, (VoxelY) * 1);
	if (r2 > ring) {
		ring = r2;
	}

	ring *= 70;
	Mheight *= 30;

	//So my idea before I forget is to use this information to cut out select for at base
	if (overhangs.GetPerlinFractal(VoxelX, VoxelY, VoxelZ) <= 0.2) { //cuts
		if (VoxelZ < (ring - 2) && VoxelZ>Mheight) {//The abs is is you want to edit the 2
				Value = 101;

		}
		else if (VoxelZ <= (ring - 1) && VoxelZ > Mheight) {
				Value = 130;
		}
		else if (VoxelZ <= (ring) && VoxelZ > Mheight) {
				Value = 131;
		}
	}
	return Value;
}
int Rings::Detail(int VoxelX, int VoxelY, int VoxelZ, int Value) {
	if ((abs(overhangs.GetPerlin(VoxelX * 20, VoxelY * 20, VoxelZ * 20)) > folliage) && Value == 3) {
		return 132;
	}
	return Value; //So the tricky part here is it can only pass 1 block and needs to place like a tree or something, which could be algebraic trees or be grown
}
