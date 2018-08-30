// Fill out your copyright notice in the Description page of Project Settings.

#include "Character_Player.h"


// Sets default values
ACharacter_Player::ACharacter_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	CameraArm->SetupAttachment(RootComponent);
	//PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	//PlayerCamera->SetupAttachment(CameraArm);

	GetCharacterMovement()->GravityScale = GravityNormal;
	GetCharacterMovement()->AirControl = 0.7f;
	GetCharacterMovement()->JumpZVelocity = JumpPower;
	GetCharacterMovement()->MaxAcceleration = 4096;
	GetCharacterMovement()->MaxWalkSpeed = 500;
}

// Called when the game starts or when spawned
void ACharacter_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacter_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacter_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveY", this, &ACharacter_Player::MoveY);
	PlayerInputComponent->BindAxis("MoveX", this, &ACharacter_Player::MoveX);
	PlayerInputComponent->BindAxis("LookY", this, &ACharacter_Player::LookY);
	PlayerInputComponent->BindAxis("LookX", this, &ACharacter_Player::LookX);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter_Player::JumpPressed);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter_Player::JumpReleased);
}

void ACharacter_Player::MoveY(float inputVal)
{
	FVector inputVector(inputVal, 0, 0);
	FRotator inputRotation = FRotator(0,0,0);
	inputRotation.Yaw = GetControlRotation().Yaw;
	inputVector = inputRotation.RotateVector(inputVector);
	AddMovementInput(inputVector);
}

void ACharacter_Player::MoveX(float inputVal)
{
	FVector inputVector(0, inputVal, 0);
	FRotator inputRotation = FRotator(0, 0, 0);
	inputRotation.Yaw = GetControlRotation().Yaw;
	inputVector = inputRotation.RotateVector(inputVector);
	AddMovementInput(inputVector);
}

void ACharacter_Player::LookY(float inputVal)
{
	AddControllerPitchInput(inputVal);
}

void ACharacter_Player::LookX(float inputVal)
{
	AddControllerYawInput(inputVal);
}

void ACharacter_Player::JumpPressed()
{
	SetGravity(GravityJump);
	SetGravityServer(GravityJump);
	ACharacter::Jump();
}

void ACharacter_Player::JumpReleased()
{
	SetGravity(GravityNormal);
	SetGravityServer(GravityNormal);
}

void ACharacter_Player::SetGravity(float NewGravity)
{
	GetCharacterMovement()->GravityScale = NewGravity;
}

bool ACharacter_Player::SetGravityServer_Validate(float NewGravity)
{
	//fix this
	return true;
}

void ACharacter_Player::SetGravityServer_Implementation(float NewGravity)
{
	SetGravity(NewGravity);
}