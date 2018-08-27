// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Voxel_World.h"
#include "GameFramework/GameModeBase.h"
#include "VoxelGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class VOXEL_API AVoxelGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	AVoxel_World* GameWorld;
	
};
