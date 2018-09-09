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
	//First I check if it is part of an island
	if (islands.GetPerlin((PosX) * 1, (PosY) * 1) > 0) {
		//Then I check which biome it is

		if (temperature.GetPerlin((PosX) * 1, (PosY) * 1) < rainfall.GetPerlin((PosX) * 1, (PosY) * 1)) {
			return 1;
		}
		else {
			return 2;
		}
	}
	else {
		return 0;
	}
	//Current plan! I will have a matrix of sorts for biomes based on temp and rainfall (
}
