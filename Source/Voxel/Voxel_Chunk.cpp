// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxel_Chunk.h"
#include <string>


// Sets default values
AVoxel_Chunk::AVoxel_Chunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	GenericVoxel = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("GenericVoxelMesh"));
	GenericVoxel->SetupAttachment(RootComponent);

	UStaticMesh* tmpMesh = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Cube.Cube"));
	if (tmpMesh != nullptr)
	{
		GenericVoxel->SetStaticMesh(tmpMesh);
	}


	NetworkData = TArray<uint16>();
	NetworkData.Init(0, 1000);
}


//Register variables for replication
void AVoxel_Chunk::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVoxel_Chunk, PosX);
	DOREPLIFETIME(AVoxel_Chunk, PosY);
	DOREPLIFETIME(AVoxel_Chunk, PosZ);

	DOREPLIFETIME(AVoxel_Chunk, NetworkData);
}

// Called when the game starts or when spawned
void AVoxel_Chunk::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		Generate();
	}

}

//probaly will not need this..
void AVoxel_Chunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		NetworkData[1] = 1;
	}

}

void AVoxel_Chunk::Init(int LocX, int LocY, int LocZ, FastNoise* noise)
{
	if (HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(1));
		PosX = LocX;
		PosY = LocY;
		PosZ = LocZ;
		TheNoise = noise;

		UpdateChunk();
	}
}

void AVoxel_Chunk::Generate()
{
	//Double check that we are the server
	if (HasAuthority())
	{
		//Generation Code here

		

		/*
		if (TheNoise != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(TheNoise->GetSeed()));
			//if (TheNoise->GetPerlin(VoxelX + (PosX * 100), VoxelY + (PosY * 100), VoxelZ + (PosZ * 100)) >= 0.05) {
				
			//}
		}
		*/

		OnRep_NetworkData();
	}

}

//Logic to update chunk... duh
void AVoxel_Chunk::UpdateChunk()
{

	GenericVoxel->ClearInstances();



	for (int8 VoxelX = 0; VoxelX < 10; VoxelX++)
	{
		for (int8 VoxelY = 0; VoxelY < 10; VoxelY++)
		{
			for (int8 VoxelZ = 0; VoxelZ < 10; VoxelZ++)
			{
				switch (ChunkData[VoxelX][VoxelY][VoxelZ])
				{
				default:

					break;
				case 1:
					GenericVoxel->AddInstance(FTransform(FRotator(0, 0, 0), FVector(VoxelX * 100, VoxelY * 100, VoxelZ * 100), FVector(1, 1, 1)));
					break;
				}
			}
		}
	}
}

void AVoxel_Chunk::OnRep_NetworkData()
{
	
	for (int I = 0; I < 1000; I++)
	{
		ChunkData[I % 10][(I / 10) % 10][I / 100] = NetworkData[I];
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(ChunkData[1][0][0]));

	UpdateChunk();
}