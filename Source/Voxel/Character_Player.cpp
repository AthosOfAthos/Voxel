// Fill out your copyright notice in the Description page of Project Settings.

#include "Character_Player.h"


// Sets default values
ACharacter_Player::ACharacter_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	bUseControllerRotationYaw = false;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	CameraArm->SetupAttachment(RootComponent);
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	PlayerCamera->SetupAttachment(CameraArm);

	GetCharacterMovement()->GravityScale = GravityNormal;
	GetCharacterMovement()->AirControl = 0.7f;
	GetCharacterMovement()->JumpZVelocity = JumpPower;
	GetCharacterMovement()->MaxAcceleration = 4096;
	GetCharacterMovement()->MaxWalkSpeed = 500;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 500, 0);

	CameraArm->TargetArmLength = 500;
	CameraArm->SetRelativeLocation(FVector(0,0,75));

	GetMesh()->SetSkeletalMesh(LoadObject<USkeletalMesh>(NULL, TEXT("/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin")));
	GetMesh()->SetRelativeLocation(FVector(0,0,-90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	AnimationBP = LoadObject<UAnimBlueprint>(NULL, TEXT("AnimBlueprint'/Game/Blueprints/Animation_Player.Animation_Player'"));
	GetMesh()->SetAnimInstanceClass(AnimationBP->GetAnimBlueprintGeneratedClass());
}

void ACharacter_Player::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacter_Player, ControlledBoard);
	DOREPLIFETIME(ACharacter_Player, IsMounted);
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

	CameraArm->SetWorldRotation(GetControlRotation());

	if (ControlledBoard != nullptr && IsMounted)
	{
		SetActorLocation(ControlledBoard->GetActorLocation());
	}

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

	PlayerInputComponent->BindAction("UseBoard", IE_Pressed, this, &ACharacter_Player::UseBoardPressed);
}

void ACharacter_Player::MoveY(float inputVal)
{
	if (ControlledBoard != nullptr && IsMounted)
	{
		ControlledBoard->AddPitch(inputVal);
	}
	else
	{
		FVector inputVector(inputVal, 0, 0);
		FRotator inputRotation = FRotator(0, 0, 0);
		inputRotation.Yaw = GetControlRotation().Yaw;
		inputVector = inputRotation.RotateVector(inputVector);
		AddMovementInput(inputVector);
	}
}

void ACharacter_Player::MoveX(float inputVal)
{
	if (ControlledBoard != nullptr && IsMounted)
	{
		ControlledBoard->AddRoll(inputVal);
	}
	else
	{
		FVector inputVector(0, inputVal, 0);
		FRotator inputRotation = FRotator(0, 0, 0);
		inputRotation.Yaw = GetControlRotation().Yaw;
		inputVector = inputRotation.RotateVector(inputVector);
		AddMovementInput(inputVector);
	}
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

void ACharacter_Player::UseBoardPressed()
{
	SpawnBoard();
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

bool ACharacter_Player::SpawnBoard_Validate()
{
	return true;
}

void ACharacter_Player::SpawnBoard_Implementation()
{
	if (HasAuthority())
	{
		if (IsMounted)
		{
			IsMounted = false;
			ControlledBoard->Destroy();
		}
		else
		{
			IsMounted = true;
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = this;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnInfo.Instigator = this;
			ControlledBoard = GetWorld()->SpawnActor<AHoverBoard>(GetActorLocation(), GetActorRotation(), SpawnInfo);
		}

		
	}
}