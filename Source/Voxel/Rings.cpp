// Fill out your copyright notice in the Description page of Project Settings.

#include "Rings.h"
#include "Voxel_World.h"
#include "VoxelGameModeBase.h"
//This class generates the known and loved skyfallen ring terrain
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


	float ring = rings.GetPerlinFractal ((VoxelX) * 1, (VoxelY) * 1);//Considering messing with X/Y ratio to create arches
	float r2 = rings.GetPerlinFractal((VoxelX) * 2, (VoxelY) * 2);
	/*if (r2 > ring) {
		ring = r2;
	}*/

	ring *= 200;
	r2 *= 200;

	//So my idea before I forget is to use this information to cut out select for at base
	if (overhangs.GetPerlinFractal(VoxelX, VoxelY, VoxelZ) <= 0.2||true) { //cuts
		if (VoxelZ < (ring - 2) && VoxelZ < (r2 - 2) && VoxelZ >(ring - 10)) {//I want to change it so that this makes rings of a certain rad (Current problem is perlin noise is smoothish 
				Value = 101;
		}
		else if (VoxelZ <= (ring - 1) && VoxelZ < (r2 - 1) && VoxelZ > (ring - 10)) {
				Value = 130;
		}
		else if (VoxelZ <= (ring) && VoxelZ < (r2) && VoxelZ > (ring - 10)) {
				Value = 131;
		}
	}
	if (Value == 131 && VoxelZ <= 0) {
		Value = 132;
	}
	return Value;
}
int Rings::Detail(int VoxelX, int VoxelY, int VoxelZ, int Value) {
	if ((abs(overhangs.GetPerlin(VoxelX * 20, VoxelY * 20, VoxelZ * 20)) > folliage)) {
		
	}
	return Value; //So the tricky part here is it can only pass 1 block and needs to place like a tree or something, which could be algebraic trees or be grown
}
