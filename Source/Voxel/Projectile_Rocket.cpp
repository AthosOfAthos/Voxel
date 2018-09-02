// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile_Rocket.h"

AProjectile_Rocket::AProjectile_Rocket()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->OnComponentHit.AddDynamic(this, &AProjectile_Rocket::OnHit);
	Collision->SetBoxExtent(FVector(50, 50, 50));
	Collision->SetCollisionProfileName(FName("BlockAll"));
	Collision->SetMobility(EComponentMobility::Movable);
	RootComponent = Collision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MesH"));
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel_Generic.Voxel_Generic")));

	UProjectileMovementComponent* Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	Projectile->ProjectileGravityScale = 1;
	Projectile->InitialSpeed = 2000;
	Projectile->Friction = 0;
	Projectile->SetUpdatedComponent(RootComponent);
}

void AProjectile_Rocket::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectile_Rocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile_Rocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority())
	{
		AVoxelGameModeBase* GameMode = (AVoxelGameModeBase*)GetWorld()->GetAuthGameMode();

		for (int VoxelX = -2; VoxelX < 2; VoxelX++)
		{
			for (int VoxelY = -2; VoxelY < 2; VoxelY++)
			{
				for (int VoxelZ = -2; VoxelZ < 2; VoxelZ++)
				{
					GameMode->GameWorld->SetBlock(GetActorLocation().X / 100, GetActorLocation().Y / 100, GetActorLocation().Z / 100, 0);
				}
			}
		}

		Destroy();
	}
}