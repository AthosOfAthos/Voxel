// Fill out your copyright notice in the Description page of Project Settings.

#include "Pillars.h"
//This class generates the known and loved skyfallen pillar terrain
Pillars::Pillars(int Seed)
{
	land = FastNoise();
	land.SetSeed(Seed);
	land.SetFrequency(0.01);
	land.SetFractalOctaves(5);

	overhangs = FastNoise();
	overhangs.SetSeed(Seed);
	overhangs.SetFrequency(0.1);
	overhangs.SetFractalGain(0.1);

	pillars = FastNoise();
	pillars.SetSeed(Seed);
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
	int Value = 0;
	if (VoxelZ < -10) {//Controls floor stop
		return 0;
	}

	float Mheight = land.GetPerlinFractal((VoxelX) * 1, (VoxelY) * 1);
	
	float pillar = pillars.GetCellular((VoxelX) * 1, (VoxelY) * 1);

	/*if (Mheight > 0.01 && pillar>0.2) { //I think this is an uh oh 
		pillar *= 50;
	}
	else {
		pillar = 0;
	}*/
	pillar *= 50;
	Mheight *= 25;

	if (VoxelZ > Mheight && pillar != 0) {//So my idea before I forget is to use this information to cut out select for at base
		if (overhangs.GetPerlinFractal(VoxelX, VoxelY, VoxelZ) <= 0.2) { //Pillar cuts
			if (VoxelZ < (pillar - 2)&& VoxelZ>(pow(std::abs(Mheight),2)*pillar/(pillar-5))) {//The abs is is you want to edit the 2
				Value = 101;

			}
			else if (VoxelZ <= (pillar-1) && VoxelZ > (pow(std::abs(Mheight), 2)*pillar / (pillar - 5))) {
				Value = 130;
			}
			else if (VoxelZ <= (pillar) && VoxelZ > (pow(std::abs(Mheight),2)*pillar/(pillar-5))) {
				Value = 131;
			}
		}
	}else if (overhangs.GetPerlinFractal(VoxelX, VoxelY, VoxelZ) <= 0.35) {//Land cave size
		if (VoxelZ < (Mheight - 2)) {
			Value = 101;

		}else if (VoxelZ <= (Mheight-1)) {
			Value = 130;
		}else if (VoxelZ <= (Mheight)) {
			Value = 131;
		}

	}
	if (Value == 131 && VoxelZ <= 0) {
		Value = 132;
	}
	return Value;
}
int Pillars::Detail(int VoxelX, int VoxelY, int VoxelZ, int Value) {
	if ((abs(overhangs.GetPerlin(VoxelX * 20, VoxelY * 20, VoxelZ * 20)) > folliage) && Value == 3) {
		return 132;
	}
	return Value; //So the tricky part here is it can only pass 1 block and needs to place like a tree or something, which could be algebraic trees or be grown
}
