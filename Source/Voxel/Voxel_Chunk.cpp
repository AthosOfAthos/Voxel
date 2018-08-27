// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxel_Chunk.h"


// Sets default values
AVoxel_Chunk::AVoxel_Chunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVoxel_Chunk::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVoxel_Chunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

