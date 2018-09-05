// Fill out your copyright notice in the Description page of Project Settings.

#include "HoverBoard.h"

AHoverBoard::AHoverBoard()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;

	BoardCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Board Collision"));
	BoardCollision->SetBoxExtent(FVector(75, 25, 10));
	BoardCollision->SetCollisionProfileName(FName("HoverBoard"));
	BoardCollision->SetMobility(EComponentMobility::Movable);
	RootComponent = BoardCollision;

	BoardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Board Mesh"));
	BoardMesh->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Board/game_board.game_board")));
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

	if (HasAuthority())
	{
		BoardRotation.Roll = FMath::FInterpTo(BoardRotation.Roll, TargetRoll, DeltaTime, RollResponse);
		BoardRotation.Yaw += BoardRotation.Roll * YawResponse * DeltaTime;
	}

	AirSpeed += FMath::Sin(FMath::DegreesToRadians(BoardRotation.Pitch)) * GravityZ * DeltaTime;
	AirSpeed += (Thrust / Mass) * DeltaTime;
	AirSpeed -= ((0.5) * (DragCoefficient * FMath::Square(AirSpeed / 10))) / Mass;
	AirSpeed = FMath::Clamp(AirSpeed, (float)0, Vne);

	//TODO
	Lift = 0;

	FVector TargetLocation = GetActorLocation();
	FVector MovementVector = FVector(AirSpeed * DeltaTime, 0, 0);
	FVector LiftVector = FVector(0, 0, Lift * DeltaTime);
	MovementVector = BoardRotation.RotateVector(MovementVector);
	TargetLocation += MovementVector;
	TargetLocation += LiftVector;
	SetActorLocation(TargetLocation, true);
	SetActorRotation(BoardRotation);
}

void AHoverBoard::AddPitch(float inputAmount)
{
	inputAmount *= -1;
	BoardRotation.Pitch += inputAmount * PitchResponse * FApp::GetDeltaTime();
	AddPitchServer(inputAmount);
}

void AHoverBoard::AddRoll(float inputAmount)
{
	AddRollServer(inputAmount);
}

bool AHoverBoard::AddPitchServer_Validate(float inputAmount)
{
	//its all good
	return true;
}

void AHoverBoard::AddPitchServer_Implementation(float inputAmount)
{
	BoardRotation.Pitch += inputAmount * PitchResponse * FApp::GetDeltaTime();
}

bool AHoverBoard::AddRollServer_Validate(float inputAmount)
{
	//its all good
	return true;
}

void AHoverBoard::AddRollServer_Implementation(float inputAmount)
{
	TargetRoll = inputAmount * 45;
}