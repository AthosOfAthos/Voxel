// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "UnrealNetwork.h"
#include "VoxelGameModeBase.h"
#include "HoverBoard.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimBlueprint.h"
#include "Projectile_Rocket.h"
#include "Speargun.h"
#include "Character_Player.generated.h"

UCLASS()
class VOXEL_API ACharacter_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacter_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UCameraComponent* PlayerCamera;
	USpringArmComponent* CameraArm;
	UAnimBlueprint* AnimationBP;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveY(float);
	void MoveX(float);
	void LookY(float);
	void LookX(float);

	void JumpPressed();
	void JumpReleased();

	void SprintPressed();
	void SprintReleased();

	void PrimaryPressed();

	void UseBoardPressed();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);

	void SetGravity(float);
	UFUNCTION(Server, Reliable, WithValidation)
	void SetGravityServer(float NewGravity);

	void SetSprint(bool NewSprint);
	UFUNCTION(Server, Reliable, WithValidation)
	void SetSprintServer(bool NewSprint);

	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnBoard();

	UFUNCTION(Server, Reliable, WithValidation)
	void FireRocket();

	UFUNCTION(Server, Reliable, WithValidation)
	void FireSpear();

	UPROPERTY(Replicated)
	AHoverBoard* ControlledBoard;
	UPROPERTY(Replicated)
	bool IsMounted = false;;

	float GravityNormal = 3;
	float GravityJump = 1.5;
	float JumpPower = 900;

	float FOV = 110;
	float FOVNormal = 110;
	float FOVSprinting = 120;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsSprinting = false;
};
