// Fill out your copyright notice in the Description page of Project Settings.

#include "Speargun.h"


// Sets default values
ASpeargun::ASpeargun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->OnComponentHit.AddDynamic(this, &ASpeargun::OnHit);
	Collision->SetBoxExtent(FVector(50, 50, 50));
	Collision->SetCollisionProfileName(FName("BlockAll"));
	Collision->SetMobility(EComponentMobility::Movable);
	RootComponent = Collision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MesH"));
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Other/game_spear.game_spear")));

	Speed = 1000;

}

// Called when the game starts or when spawned
void ASpeargun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpeargun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Speed *= (1 - Friction);
	SetActorRotation(GetActorRotation() + GetActorForwardVector().Rotation()*DeltaTime);
	SetActorLocation(GetActorLocation() + (GetActorForwardVector()*Speed*DeltaTime) + FVector(0, 0, -Gravity)*DeltaTime);

}
void ASpeargun::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority())
	{
		//Speed = 0;
		//Destroy();
	}
}

