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
	TestAirship = GetWorld()->SpawnActor<AAirship_Generic>(FVector(0, 0, 4000), FRotator(0, 0, 0), SpawnInfo);
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

				if (Players[i]->GetPawn() == nullptr)
					SpawnPlayer(Players[i]);
			}
		}



		
	}
}

void AVoxelGameModeBase::SpawnPlayer(APlayerController* Player)
{
	if (Player->GetPawn() != nullptr)
		Player->GetPawn()->Destroy();

	if (TestAirship == nullptr)
		return;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector SpawnLocation = TestAirship->GetActorLocation();
	SpawnLocation.Z += 100;
	APawn* NewPawn = GetWorld()->SpawnActor<ACharacter_Player>(SpawnLocation, FRotator(0, 0, 0), SpawnInfo);

	Player->Possess(NewPawn);
}

void AVoxelGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Players.Add(NewPlayer);
}