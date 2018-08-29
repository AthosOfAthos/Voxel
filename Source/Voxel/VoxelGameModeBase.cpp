// Fill out your copyright notice in the Description page of Project Settings.

#include "VoxelGameModeBase.h"

AVoxelGameModeBase::AVoxelGameModeBase()
{
	DefaultPawnClass = ACharacter_Player::StaticClass();
	PlayerControllerClass = AController_Player::StaticClass();
}

void AVoxelGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//Spawn world
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GameWorld = GetWorld()->SpawnActor<AVoxel_World>(FVector(0,0,0), FRotator(0,0,0), SpawnInfo);
}


