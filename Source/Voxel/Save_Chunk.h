// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Save_Chunk.generated.h"

/**
 * 
 */
UCLASS()
class VOXEL_API USave_Chunk : public USaveGame
{
	GENERATED_BODY()

	uint16 SavedChunk[1000];
	
	
};
