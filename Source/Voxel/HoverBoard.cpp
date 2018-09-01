// Fill out your copyright notice in the Description page of Project Settings.

#include "HoverBoard.h"

AHoverBoard::AHoverBoard()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	BoardCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Board Collision"));
	BoardCollision->SetBoxExtent(FVector(75, 25, 10));
	BoardCollision->SetCollisionProfileName(FName("HoverBoard"));
	BoardCollision->SetMobility(EComponentMobility::Movable);
	RootComponent = BoardCollision;

	BoardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Board Mesh"));
	BoardMesh->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Board/board.board")));
	BoardMesh->SetMobility(EComponentMobility::Movable);
	BoardMesh->SetCollisionProfileName(FName("NoCollision"));
	BoardMesh->SetupAttachment(RootComponent);
}

void AHoverBoard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHoverBoard, BoardRotation);
}

void AHoverBoard::BeginPlay()
{
	Super::BeginPlay();
	
	BoardRotation = GetActorRotation();
}

void AHoverBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AirSpeed = 900;

	BoardRotation.Yaw += BoardRotation.Roll * YawResponse * DeltaTime;

	FVector TargetLocation = GetActorLocation();
	FVector MovementVector = FVector(AirSpeed * DeltaTime, 0, 0);
	MovementVector = BoardRotation.RotateVector(MovementVector);
	TargetLocation += MovementVector;
	SetActorLocation(TargetLocation, true);
	SetActorRotation(BoardRotation);
}

bool AHoverBoard::AddPitch_Validate(float inputAmount)
{
	//its all good
	return true;
}

void AHoverBoard::AddPitch_Implementation(float inputAmount)
{
	inputAmount *= -1;
	BoardRotation.Pitch += inputAmount * PitchResponse * FApp::GetDeltaTime();
}

bool AHoverBoard::AddRoll_Validate(float inputAmount)
{
	//its all good
	return true;
}

void AHoverBoard::AddRoll_Implementation(float inputAmount)
{
	BoardRotation.Roll = inputAmount * 45;
}