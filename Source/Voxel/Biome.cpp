// Fill out your copyright notice in the Description page of Project Settings.

#include "Biome.h"

Biome::Biome(int WorldSeed)
{

	Seed = WorldSeed;
	temperature = FastNoise();
	temperature.SetSeed(Seed);
	temperature.SetFrequency(0.001);

	rainfall = FastNoise();
	rainfall.SetSeed(Seed + 1);
	rainfall.SetFrequency(0.01);

	islands = FastNoise();
	islands.SetSeed(Seed + 2);
	islands.SetFrequency(0.01);
}

Biome::~Biome()
{
}
int Biome::GetBiome(int PosX, int PosY, int PosZ)
{
	int islandSize = 5;
	//First I check if it is part of an island
	if (PosX%islandSize == 0 || PosY % islandSize == 0) {
		return 0; //Sepeartes out my grid
	}
	/*
	o--o--o--o--o
	-------------
	o--x--x--x--o
	-------------
	o--x--x--x--o
	-------------
	o--x--x--x--o
	-------------
	o--o--o--o--o
	*/
	else {
		//Now I need this to return only the same value for inner row
		int fixedX = PosX - PosX % islandSize;//Should return only the island location!
		int fixedY = PosY - PosY % islandSize;
		int fixedZ = PosZ - PosZ % islandSize;
		if (temperature.GetPerlin((fixedX) * 1, (fixedY) * 1) < rainfall.GetPerlin((fixedX) * 1, (fixedY) * 1)) {
			return 1;
		}
		else {
			return 2;
		}
	}
	return 0;
}

	//Current plan! I will have a matrix of sorts for biomes based on temp and rainfall 