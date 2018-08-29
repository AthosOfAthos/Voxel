// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxel_Chunk.h"
#include <cmath>


AVoxel_Chunk::AVoxel_Chunk()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	GenericVoxel = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("GenericVoxelMesh"));
	GenericVoxel->SetupAttachment(RootComponent);

	VoxelMesh.Init(nullptr, 5);
	VoxelMesh[0] = GenericVoxel;

	//Find generic Voxel mesh world loaded for us
	VoxelMesh[0]->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Cube.Cube")));


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

uint16 AVoxel_Chunk::GetBlock(int VoxelX, int VoxelY, int VoxelZ)
{
	return ChunkData[VoxelX % 10][VoxelY % 10][VoxelZ % 10];
}

void AVoxel_Chunk::SetBlock(int VoxelX, int VoxelY, int VoxelZ, int Id)
{
	if (HasAuthority())
	{
		VoxelX = VoxelX % 10;
		VoxelY = VoxelY % 10;
		VoxelZ = VoxelZ % 10;
		NetworkData[VoxelX + (VoxelY * 10) + (VoxelZ * 100)] = Id;
		OnRep_NetworkData();
	}
}

void AVoxel_Chunk::Generate()
{
	if (HasAuthority())
	{
		//Generation Code here 
		/*
		Important note from the editor. There is a lot of generation code and I need to test a lot of things I want to test
		As such please don't mess with the format I've set up unless we speak first <KinginYellow>
		Basically Generate iterates through all blocks in the chunk and then decides if it should be block or not
		As such if testing something choose to edit the int value (via a function which takes the pos and returns 1 or 0)
		*/

		for (int8 VoxelX = 0; VoxelX < 10; VoxelX++)
		{
			for (int8 VoxelY = 0; VoxelY < 10; VoxelY++)
			{
				for (int8 VoxelZ = 0; VoxelZ < 10; VoxelZ++)
				{
					//NetworkData[VoxelX + (VoxelY * 10) + (VoxelZ * 100)] = Shape(VoxelX, VoxelY, VoxelZ);
					int Value = Height(VoxelX, VoxelY, VoxelZ);
					NetworkData[VoxelX + (VoxelY * 10) + (VoxelZ * 100)] = Value;

				}
			}
		}


		OnRep_NetworkData();
	}
}
int AVoxel_Chunk::Noise(int VoxX, int VoxY, int VoxZ)
{
	if (TheNoise != nullptr) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(TheNoise->GetPerlin(VoxelX + (PosX * 100), VoxelY + (PosY * 100), VoxelZ + (PosZ * 100))));
		if (std::abs(TheNoise->GetPerlin(VoxX + (PosX * 100), VoxY + (PosY * 100), VoxZ + (PosZ * 100))) >= 0.04)
		{
			 return 1;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
	

}
int AVoxel_Chunk::Shape(int VoxX, int VoxY, int VoxZ)
{
	int c = 10;
	int a = 12; 
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat((pow(PosX * 100 + VoxX, 2) + pow(PosY * 100 + VoxY, 2) + pow(PosZ * 100 + VoxZ, 2))));
	/*if (pow(PosX*100 + VoxX,2)+ pow(PosY * 100 + VoxY, 2)+ pow(PosZ * 100 + VoxZ, 2) <= pow(c,2)){
		return 1;
	}
	else */
	if (VoxX == 1) {
		return 1;
	}
	else {
		return 0;
	}
}
int AVoxel_Chunk::Height(int VoxX, int VoxY, int VoxZ) 
{
	float Height = TheNoise->GetPerlin((VoxX + (PosX * 10)) * 1, (VoxY + (PosY * 10)) * 1);
	Height *= 20;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(Height));
	
	if ((VoxZ + (PosZ * 1000)) < Height)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void AVoxel_Chunk::UpdateChunk()
{
	VoxelMesh[0]->ClearInstances();

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
					VoxelMesh[0]->AddInstance(FTransform(FRotator(0, 0, 0), FVector(VoxelX * 100, VoxelY * 100, VoxelZ * 100), FVector(1, 1, 1)));
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