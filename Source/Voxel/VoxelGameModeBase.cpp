// Fill out your copyright notice in the Description page of Project Settings.

#include "VoxelGameModeBase.h"

AVoxelGameModeBase::AVoxelGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bStartPlayersAsSpectators = true;

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

void AVoxelGameModeBase::Tick(float DeltaTime)
{
	if (GameWorld != nullptr)
	{

		if (Players.Num() != 0)
		{
			for (int i = 0; i < Players.Num(); i++)
			{
				if (Players[i]->GetPawnOrSpectator() != nullptr)
					GameWorld->PlayerLocations[i] = Players[i]->GetPawnOrSpectator()->GetActorLocation();
			}
		}



		
	}
}

void AVoxelGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Players.Add(NewPlayer);
	//RestartPlayer(NewPlayer);
}