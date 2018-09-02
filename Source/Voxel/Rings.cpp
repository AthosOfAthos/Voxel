// Fill out your copyright notice in the Description page of Project Settings.

#include "Rings.h"
//This class generates the known and loved skyfallen pillar terrain
Rings::Rings()
{
	cubic = FastNoise();
	cubic.SetSeed(5);
	cubic.SetFrequency(0.03);
	cubic.SetFractalGain(0.1);

	perlin = FastNoise();
	perlin.SetSeed(5);
	perlin.SetFrequency(0.07);
	folliage = 0.5;
}

Rings::~Rings()
{
}
/*
Very Important! Pass this method only the total position (voxel pos + chunk pos * 10
*/
int Rings::Generate(int VoxelX, int VoxelY, int VoxelZ) {
	int Value = Land(VoxelX, VoxelY, VoxelZ);

	Value = Detail(VoxelX, VoxelY, VoxelZ, Value);

	return Value;
}
int Rings::Land(int VoxelX, int VoxelY, int VoxelZ) {
	float Mheight = cubic.GetCubic((VoxelX) * 1, (VoxelY) * 1);
	Mheight *= 30;
	if (VoxelZ <= (Mheight)) {
		return 2;
	}
	//To make a ring. Pick a point in space, weight against the distance to that for all chunks 
	//Torus (pow(c-sqrt(pow(VoxelX,2)+pow(VoxelY,2)),2)>4)
	//Find a point with noise
	int c = 15;
	int a = 5;
	if ((pow(c-sqrt(pow(VoxelZ,2)+pow(VoxelY,2)),2) + pow(VoxelX,2)<pow(a,2))) {//The equation to my point, starting with zero zero for a test 
		return 3;
	}
	if ((pow(c+4 - sqrt(pow(VoxelZ, 2) + pow(VoxelX, 2)), 2) + pow(VoxelY, 2) < pow(a, 2))) {//The equation to my point, starting with zero zero for a test 
		return 3;
	}
	return 0;
}
int Rings::Detail(int VoxelX, int VoxelY, int VoxelZ, int Value) {
	if ((abs(perlin.GetPerlin(VoxelX * 20, VoxelY * 20, VoxelZ * 20)) > folliage) && Value == 3) {
		return 4;
	}
	return Value; //So the tricky part here is it can only pass 1 block and needs to place like a tree or something, which could be algebraic trees or be grown
}
