// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxel_Chunk.h"
#include <cmath>
#include <vector>
#include <algorithm>


AVoxel_Chunk::AVoxel_Chunk()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	VoxelMesh.Init(nullptr, 5);

	GenericVoxel = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Generic Voxel"));
	GenericVoxel->SetupAttachment(RootComponent);
	VoxelMesh[1] = GenericVoxel;

	GenericVoxel = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Stone"));
	GenericVoxel->SetupAttachment(RootComponent);
	VoxelMesh[2] = GenericVoxel;

	GenericVoxel = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Dirt"));
	GenericVoxel->SetupAttachment(RootComponent);
	VoxelMesh[3] = GenericVoxel;

	GenericVoxel = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Sand"));
	GenericVoxel->SetupAttachment(RootComponent);
	VoxelMesh[4] = GenericVoxel;

	//Find generic Voxel mesh world loaded for us
	VoxelMesh[1]->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_Generic.Voxel_Generic")));
	VoxelMesh[2]->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_Stone.Voxel_Stone")));
	VoxelMesh[3]->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_Dirt.Voxel_Dirt")));
	VoxelMesh[4]->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_Sand.Voxel_Sand")));

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
	return ChunkData[VoxelX % 10][VoxelY % 10][VoxelZ % 10];//doubt
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

void AVoxel_Chunk::SaveChunk()
{
	//Todo
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
		//Pass 1 pops the blocks in place
		for (int8 VoxelX = 0; VoxelX < 10; VoxelX++)
		{
			for (int8 VoxelY = 0; VoxelY < 10; VoxelY++)
			{
				for (int8 VoxelZ = 0; VoxelZ < 10; VoxelZ++)
				{
					//NetworkData[VoxelX + (VoxelY * 10) + (VoxelZ * 100)] = Shape(VoxelX, VoxelY, VoxelZ);
					std::vector<int> pos{ VoxelX, VoxelY, VoxelZ };
					int Value = Mountains(pos);

					/*if (Value == 0) {
						Value = Noise(VoxelX, VoxelY, VoxelZ);
						int i = 1;
					}*/
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
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat((VoxZ + (PosZ * 10))));
		if ((std::abs(TheNoise->GetPerlin(VoxX + (PosX * 100), VoxY + (PosY * 100), VoxZ + (PosZ * 100))) <= 0.01)&& (VoxZ + (PosZ * 10))>15)
		{
			 return 3;
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
	int Type = 1;
	int c = 18;
	int a = 20;
	switch (Type) {
	case 0:
		if (pow((PosX * 10) + VoxX, 2) + pow((PosY * 10) + VoxY, 2) + pow((PosZ * 10) + VoxZ, 2) <= pow(c, 2)) {
			return 1;
		}
	case 1:
		if (((int)pow(c-sqrt(pow((PosX*10) +VoxX,2) +pow((PosY*10)+VoxY,2)), 2)+pow((PosZ*10)+VoxZ,2)==pow(a,2))) {
			return 1;
		}
	}

	
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat((pow(PosX * 100 + VoxX, 2) + pow(PosY * 100 + VoxY, 2) + pow(PosZ * 100 + VoxZ, 2))));
	
	/*if ((VoxX%10 == 0) && (VoxY%10 == 0) && (VoxZ%10 == 0)) {
		return 1;
	}else {
		return 0;
	}*/
	return 0;
}
int AVoxel_Chunk::Height(int VoxX, int VoxY, int VoxZ) 
{
	float Height = TheNoise->GetPerlinFractal((VoxX + (PosX * 10)) * 1, (VoxY + (PosY * 10)) * 1);
	Height *= 20;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(Height));
	
	if ((VoxZ + (PosZ * 10)) < (Height-1))
	{
		return 2;
	}
	else if ((VoxZ + (PosZ * 10)) < Height) {
		return 3;
	}
	else
	{
		return 0;
	}
}
int AVoxel_Chunk::Mountains(std::vector<int> vox) {
	//Before I forget this takes a 2d height map and drops it on 3d noise
	
	float Mheight = TheNoise->GetPerlinFractal((vox[0] + (PosX * 10)) * 1, (vox[1] + (PosY * 10)) * 1);
	Mheight *= 20;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(Height));

	if ((vox[2] + (PosZ * 10)) < (Mheight))
	{
		//This means we would normally solid fill under it
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(TheNoise->GetPerlin(((vox[0] + (PosX * 10)) * 1, (vox[1] + (PosY * 10)) * 1), (vox[2] + (PosZ * 10)) * 1)));
		if ((std::abs(TheNoise->GetPerlin(vox[0] + (PosX * 100), vox[1] + (PosY * 100), vox[2] + (PosZ * 100))) <= 0.1)) {
			return 2;
		}
		else {
			return 0;
		}
		
	}
	return 0;
}

void AVoxel_Chunk::UpdateChunk()
{
	for (int i = 0; i < 5; i++)
	{
		if (VoxelMesh[i] != nullptr)
			VoxelMesh[i]->ClearInstances();
	}

	for (int8 VoxelX = 0; VoxelX < 10; VoxelX++)
	{
		for (int8 VoxelY = 0; VoxelY < 10; VoxelY++)
		{
			for (int8 VoxelZ = 0; VoxelZ < 10; VoxelZ++)
			{
				int i = ChunkData[VoxelX][VoxelY][VoxelZ];
				if (VoxelMesh[i] != nullptr)
					VoxelMesh[i]->AddInstance(FTransform(FRotator(0, 0, 0), FVector(VoxelX * 100, VoxelY * 100, VoxelZ * 100), FVector(1, 1, 1)));
			}
		}
	}
}

void AVoxel_Chunk::OnRep_NetworkData()
{
	//Make easy to work with
	for (int I = 0; I < 1000; I++)
	{
		ChunkData[I % 10][(I / 10) % 10][I / 100] = NetworkData[I];//not sure if this works
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(ChunkData[1][0][0]));

	UpdateChunk();
}