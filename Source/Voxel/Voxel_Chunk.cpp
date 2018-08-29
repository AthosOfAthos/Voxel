// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxel_Chunk.h"
#include "Voxel_World.h"

AVoxel_Chunk::AVoxel_Chunk()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	GenericVoxel = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("GenericVoxelMesh"));
	GenericVoxel->SetupAttachment(RootComponent);

	UStaticMesh* tmpMesh = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Cube.Cube"));
	if (tmpMesh != nullptr)
	{
		GenericVoxel->SetStaticMesh(tmpMesh);
	}

	for (int I = 0; I < 1000; I++)
	{
		NetworkData[I] = 0;
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
	
	if (HasAuthority())
	{

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
		PosX = LocX;
		PosY = LocY;
		PosZ = LocZ;
		TheNoise = noise;
		Generate();
	}
}

uint16 AVoxel_Chunk::GetBlock(int BlockX, int BlockY, int BlockZ)
{
	return ChunkData[BlockX % 100][BlockY % 100][BlockZ % 100];
}

void AVoxel_Chunk::Generate()
{
	//Double check that we are the server
	if (HasAuthority())
	{
		//Generation Code here

		for (int8 VoxelX = 0; VoxelX < 10; VoxelX++)
		{
			for (int8 VoxelY = 0; VoxelY < 10; VoxelY++)
			{
				for (int8 VoxelZ = 0; VoxelZ < 10; VoxelZ++)
				{
					if (TheNoise != nullptr) {
						//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(TheNoise->GetPerlin(VoxelX + (PosX * 100), VoxelY + (PosY * 100), VoxelZ + (PosZ * 100))));
						if (std::abs(TheNoise->GetPerlin(VoxelX + (PosX * 100), VoxelY + (PosY * 100), VoxelZ + (PosZ * 100))) >= 0.04)
						{
							NetworkData[VoxelX + VoxelY * 10 + VoxelZ * 100] = 1;
						}
					}
				}
			}
		}

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

bool AVoxel_Chunk::IsOccluded(int VoxelX, int VoxelY, int VoxelZ)
{
	//Cast<AVoxel_World>(GetOwner())->GetBlock(0, 0, 0);

	return false;
}

void AVoxel_Chunk::UpdateChunk()
{

	for (int8 VoxelX = 0; VoxelX < 10; VoxelX++)
	{
		for (int8 VoxelY = 0; VoxelY < 10; VoxelY++)
		{
			for (int8 VoxelZ = 0; VoxelZ < 10; VoxelZ++)
			{
				if (!IsOccluded(VoxelX, VoxelY, VoxelZ))
				{
					RenderData[VoxelX][VoxelY][VoxelZ] = ChunkData[VoxelX][VoxelY][VoxelZ];
				}
				else
				{
					RenderData[VoxelX][VoxelY][VoxelZ] = 0;
				}
			}
		}
	}



	GenericVoxel->ClearInstances();

	for (int8 VoxelX = 0; VoxelX < 10; VoxelX++)
	{
		for (int8 VoxelY = 0; VoxelY < 10; VoxelY++)
		{
			for (int8 VoxelZ = 0; VoxelZ < 10; VoxelZ++)
			{
				switch (RenderData[VoxelX][VoxelY][VoxelZ])
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
	//Make easy to work with
	for (int I = 0; I < 1000; I++)
	{
		ChunkData[I % 10][(I / 10) % 10][I / 100] = NetworkData[I];
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(ChunkData[1][0][0]));

	UpdateChunk();
}