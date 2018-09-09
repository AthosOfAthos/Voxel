// Fill out your copyright notice in the Description page of Project Settings.

#include "Airship_Generic.h"

AAirship_Generic::AAirship_Generic()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;
	
	AirShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Airship Mesh"));
	AirShipMesh->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Airship/Airship_Test.Airship_Test")));
	AirShipMesh->SetMobility(EComponentMobility::Movable);
	AirShipMesh->SetCollisionProfileName(FName("BlockAll"));
	RootComponent = AirShipMesh;

	AirSpeed = 500;
}

void AAirship_Generic::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAirship_Generic, Attitude);
	DOREPLIFETIME(AAirship_Generic, AirSpeed);
}

void AAirship_Generic::BeginPlay()
{
	Super::BeginPlay();
	
	Attitude = GetActorRotation();
}

void AAirship_Generic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	SetActorRotation(Attitude);
	FVector TargetLocation = GetActorLocation();
	SetActorLocation(TargetLocation);
}

