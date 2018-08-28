// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxel_Chunk.h"


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


	//DELETE THIS
	UpdateChunk();
	
}

//probaly will not need this..
void AVoxel_Chunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		NetworkData[0] = 0;
	}

}

void AVoxel_Chunk::Init(int LocX, int LocY, int LocZ, int WorldSeed)
{
	if (HasAuthority())
	{
		PosX = LocX;
		PosY = LocY;
		PosZ = LocZ;
	}
}

void AVoxel_Chunk::Generate()
{
	//Double check that we are the server
	if (HasAuthority())
	{
		//Generation Code here

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
				
				GenericVoxel->AddInstance(FTransform(FRotator(0, 0, 0), FVector(VoxelX * 100, VoxelY * 100, VoxelZ * 100), FVector(1, 1, 1)));
			}
		}
	}
}

void AVoxel_Chunk::OnRep_NetworkData()
{
	Destroy();
}