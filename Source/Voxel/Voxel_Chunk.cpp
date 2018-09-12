// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxel_Chunk.h"
#include "Voxel_World.h"

AVoxel_Chunk::AVoxel_Chunk()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	VoxelMesh.Init(nullptr, 200);
	UInstancedStaticMeshComponent* GenericVoxel;

	GenericVoxel = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Grass Foliage"));
	GenericVoxel->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_GrassFoliage.Voxel_GrassFoliage")));
	GenericVoxel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GenericVoxel->SetMobility(EComponentMobility::Movable);
	GenericVoxel->SetupAttachment(RootComponent);
	VoxelMesh[50] = GenericVoxel;

	GenericVoxel = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Generic Voxel"));
	GenericVoxel->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_Generic.Voxel_Generic")));
	GenericVoxel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GenericVoxel->SetMobility(EComponentMobility::Movable);
	GenericVoxel->SetupAttachment(RootComponent);
	VoxelMesh[100] = GenericVoxel;

	GenericVoxel = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Stone"));
	GenericVoxel->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_Stone.Voxel_Stone")));
	GenericVoxel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GenericVoxel->SetMobility(EComponentMobility::Movable);
	GenericVoxel->SetupAttachment(RootComponent);
	VoxelMesh[101] = GenericVoxel;

	GenericVoxel = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Dirt"));
	GenericVoxel->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_Dirt.Voxel_Dirt")));
	GenericVoxel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GenericVoxel->SetMobility(EComponentMobility::Movable);
	GenericVoxel->SetupAttachment(RootComponent);
	VoxelMesh[130] = GenericVoxel;

	GenericVoxel = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Grass"));
	GenericVoxel->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_Grass.Voxel_Grass")));
	GenericVoxel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GenericVoxel->SetMobility(EComponentMobility::Movable);
	GenericVoxel->SetupAttachment(RootComponent);
	VoxelMesh[131] = GenericVoxel;

	GenericVoxel = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Sand"));
	GenericVoxel->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_Sand.Voxel_Sand")));
	GenericVoxel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GenericVoxel->SetMobility(EComponentMobility::Movable);
	GenericVoxel->SetupAttachment(RootComponent);
	VoxelMesh[132] = GenericVoxel;

	for (int i = 0; i < 27000; i++)
	{
		ChunkData[i] = 0;
		RenderData[i] = 0;
	}
}

//Register variables for replication
void AVoxel_Chunk::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVoxel_Chunk, ChunkChanged);
}

// Called when the game starts or when spawned
void AVoxel_Chunk::BeginPlay()
{
	Super::BeginPlay();
	
	PosX = GetActorLocation().X / 3000;
	PosY = GetActorLocation().Y / 3000;
	PosZ = GetActorLocation().Z / 3000;

	if (!ChunkChanged)
	{
		
	}
	else
	{
		//TODO: SYNC CHUNK
	}

}

void AVoxel_Chunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (NeedsGeneration && GetOwner() != nullptr)
	{
		AVoxel_World* World = (AVoxel_World*)GetOwner();
		if (HasGenerationTask)
		{
			if (World->GenerationThreads[GenerationThreadID]->HasCompleted)
			{
				World->GenerationThreads[GenerationThreadID]->Confirm();
				NeedsGeneration = false;
				HasGenerationTask = false;
				NeedsUpdate = true;
			}
		}
		else
		{
			FindThread();
		}
	}

	if (NeedsUpdate)
	{
		NeedsUpdate = false;
		UpdateChunk();
	}
}

void AVoxel_Chunk::FindThread()
{
	AVoxel_World* World = (AVoxel_World*)GetOwner();
	int TotalThreads = World->GenerationThreads.Num();
	for (int i = 0; i < TotalThreads; i++)
	{
		if (World->GenerationThreads[i]->Start(ChunkData, PosX, PosY, PosZ))
		{
			HasGenerationTask = true;
			GenerationThreadID = i;
			return;
		}
	}
}

uint16 AVoxel_Chunk::GetBlock(int VoxelX, int VoxelY, int VoxelZ)
{
	VoxelX = VoxelX % 30;
	if (PosX < 0)
		VoxelX += 29;
	VoxelY = VoxelY % 30;
	if (PosY < 0)
		VoxelY += 29;
	VoxelZ = VoxelZ % 30;
	if (PosZ < 0)
		VoxelZ += 29;

	return ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)];
}

void AVoxel_Chunk::SetBlock_Implementation(int VoxelX, int VoxelY, int VoxelZ, int Id)
{
	VoxelX = VoxelX % 30;
	if (PosX < 0)
		VoxelX += 29;
	VoxelY = VoxelY % 30;
	if (PosY < 0)
		VoxelY += 29;
	VoxelZ = VoxelZ % 30;
	if (PosZ < 0)
		VoxelZ += 29;

	ChunkData[VoxelX + (VoxelY * 30) + (VoxelZ * 900)] = Id;
	ChunkChanged = true;
	NeedsUpdate = true;
}

bool AVoxel_Chunk::IsLocalOccluded(int VoxelX, int VoxelY, int VoxelZ)
{
	//chunk edges MUST NOT be occluded
	if (VoxelX == 0 || VoxelX == 29 || VoxelY == 0 || VoxelY == 29 || VoxelZ == 0 || VoxelZ == 29)
		return false;

	if (ChunkData[FMath::Clamp(VoxelX + 1, 0, 29) + FMath::Clamp(VoxelY, 0, 29) * 30 + FMath::Clamp(VoxelZ, 0, 29) * 900] <= 100)
		return false;
	if (ChunkData[FMath::Clamp(VoxelX - 1, 0, 29) + FMath::Clamp(VoxelY, 0, 29) * 30 + FMath::Clamp(VoxelZ, 0, 29) * 900] <= 100)
		return false;
	if (ChunkData[FMath::Clamp(VoxelX, 0, 29) + FMath::Clamp(VoxelY + 1, 0, 29) * 30 + FMath::Clamp(VoxelZ, 0, 29) * 900] <= 100)
		return false;
	if (ChunkData[FMath::Clamp(VoxelX, 0, 29) + FMath::Clamp(VoxelY - 1, 0, 29) * 30 + FMath::Clamp(VoxelZ, 0, 29) * 900] <= 100)
		return false;
	if (ChunkData[FMath::Clamp(VoxelX, 0, 29) + FMath::Clamp(VoxelY, 0, 29) * 30 + FMath::Clamp(VoxelZ + 1, 0, 29) * 900] <= 100)
		return false;
	if (ChunkData[FMath::Clamp(VoxelX, 0, 29) + FMath::Clamp(VoxelY, 0, 29) * 30 + FMath::Clamp(VoxelZ - 1, 0, 29) * 900] <= 100)
		return false;
	
	return true;
}

void AVoxel_Chunk::UpdateChunk()
{
	for (int8 VoxelX = 0; VoxelX < 30; VoxelX++)
	{
		for (int8 VoxelY = 0; VoxelY < 30; VoxelY++)
		{
			for (int8 VoxelZ = 0; VoxelZ < 30; VoxelZ++)
			{
				//Oclude blocks
				if (IsLocalOccluded(VoxelX, VoxelY, VoxelZ))
					RenderData[VoxelX + VoxelY * 30 + VoxelZ * 900] = 0;
				else
					RenderData[VoxelX + VoxelY * 30 + VoxelZ * 900] = ChunkData[VoxelX + VoxelY * 30 + VoxelZ * 900];

				//Grass
				//TODO reach into chunks below
				if ((VoxelZ - 1) < 0)
				{

				}
				else
				{
					if (ChunkData[VoxelX + VoxelY * 30 + ((VoxelZ - 1) * 900)] == 131)
					{
						RenderData[VoxelX + VoxelY * 30 + VoxelZ * 900] = 50;
					}
				}
			}
		}
	}

	for (int i = 0; i < 150; i++)
	{
		if (VoxelMesh[i] != nullptr)
			VoxelMesh[i]->ClearInstances();
	}

	for (int8 VoxelX = 0; VoxelX < 30; VoxelX++)
	{
		for (int8 VoxelY = 0; VoxelY < 30; VoxelY++)
		{
			for (int8 VoxelZ = 0; VoxelZ < 30; VoxelZ++)
			{
				int i = RenderData[VoxelX + VoxelY * 30 + VoxelZ * 900];
				switch (i)
				{
				case 50:
					//Voxel_GrassFoliage
					if (VoxelMesh[i] != nullptr)
						VoxelMesh[i]->AddInstance(FTransform(FRotator(0, 0, 0), FVector(VoxelX * 100, VoxelY * 100, VoxelZ * 100), FVector(1, 1, FMath::RandRange(0.5f, 1.5f))));
					break;
				default:
					//std Voxel
					if (VoxelMesh[i] != nullptr)
						VoxelMesh[i]->AddInstance(FTransform(FRotator(0, 0, 0), FVector(VoxelX * 100, VoxelY * 100, VoxelZ * 100), FVector(1, 1, 1)));
					break;
				}
			}
		}
	}
	
}