// Fill out your copyright notice in the Description page of Project Settings.

#include "Biome.h"

Biome::Biome(int WorldSeed)
{

	Seed = WorldSeed;
	temperature = FastNoise();
	temperature.SetSeed(Seed);
	temperature.SetFrequency(0.01);

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
	int islandSize = 16; //Land of 5x5 to 6x6 looked good
	int waterSize = 5; 
	//First I check if it is part of an island
	if (abs((PosX%islandSize)) < waterSize || abs((PosY % islandSize)) < waterSize || abs((PosX%islandSize)) > (islandSize - waterSize) || abs((PosY % islandSize)) > (islandSize - waterSize)) {
		return 0; //Sepeartes out my grid 
	}
	else if (abs((PosX%islandSize)) <= waterSize || abs((PosY % islandSize)) <= waterSize|| abs((PosX%islandSize)) >= (islandSize-waterSize) || abs((PosY % islandSize)) >= (islandSize - waterSize)) {
		return 3;
	}
	else {
		int fixedX = (PosX - (PosX% islandSize));//Should return only the island location! (I don't think it did)
		int fixedY = (PosY - (PosY% islandSize));
		if (temperature.GetPerlin(fixedX, fixedY)!=0) {
			return 1;
		}
		else {
			return 2;
		}
	}
	return 0;
}

	//Current plan! I will have a matrix of sorts for biomes based on temp and rainfall 