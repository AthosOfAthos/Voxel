// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Character_Player.generated.h"

UCLASS()
class VOXEL_API ACharacter_Player : public ACharacter
{
	GENERATED_BODY()

	GetCharacterMovement()->GravityScale = 3;
	GetCharacterMovement()->AirControl = 0.7f;
	GetCharacterMovement()->JumpZVelocity = 700;
	GetCharacterMovement()->MaxAcceleration = 4096;
	GetCharacterMovement()->MaxWalkSpeed = 400;

public:
	// Sets default values for this character's properties
	ACharacter_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveY(float);
	void MoveX(float);
	void LookY(float);
	void LookX(float);

	void JumpPressed();
	void JumpReleased();
	
	
};
