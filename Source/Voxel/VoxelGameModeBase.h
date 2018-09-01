// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Voxel_World.h"
#include "Character_Player.h"
#include "Controller_Player.h"
#include "GameFramework/GameModeBase.h"
#include "VoxelGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class VOXEL_API AVoxelGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AVoxelGameModeBase();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
public:
	AVoxel_World* GameWorld;
	TArray<APlayerController*> Players;
	
};
