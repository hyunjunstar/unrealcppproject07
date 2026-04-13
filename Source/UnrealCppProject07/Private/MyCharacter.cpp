// MyCharacter.cpp


#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsulecomp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsulecomp);
	Capsulecomp->SetSimulatePhysics(false);

	Springcomp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	Springcomp->SetupAttachment(RootComponent);
	Springcomp->TargetArmLength = 300.0f;
	Springcomp->bUsePawnControlRotation = false;

	Cameracomp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Cameracomp->SetupAttachment(Springcomp);
	Cameracomp->bUsePawnControlRotation = false;

	MoveInput = FVector2D::ZeroVector;
	MoveSpeed = 500.0f;
	TurnSpeed = 100.0f;
	LookSpeed = 100.0f;
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Move = FVector(MoveInput.X, MoveInput.Y, 0.0f);
		/*(GetActorForwardVector() * MoveInput.Y) +
		(GetActorRightVector() * MoveInput.X);*/

	if (!Move.IsNearlyZero())
	{
		AddActorLocalOffset(Move * MoveSpeed * DeltaTime, true);
	}

	if (!FMath::IsNearlyZero(LookInput.X))
	{
		float Yawvalue = LookInput.X * TurnSpeed * DeltaTime;
		AddActorLocalRotation(FRotator(0.0f, Yawvalue, 0.0f));
	}

	if (!FMath::IsNearlyZero(LookInput.Y))
	{
		float Pitchvalue = LookInput.Y * LookSpeed * DeltaTime;

		FRotator SpringArmRotation = Springcomp->GetRelativeRotation();

		SpringArmRotation.Pitch = FMath::Clamp(
			SpringArmRotation.Pitch - Pitchvalue,
			-80.0f,
			80.0f
		);

		Springcomp->SetRelativeRotation(SpringArmRotation);
	}

	MoveInput = FVector2D::ZeroVector;
	LookInput = FVector2D::ZeroVector;
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::Move
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::Look
				);
			}
		}

	}
}

void AMyCharacter::Move(const FInputActionValue & value)
{
	MoveInput = value.Get<FVector2D>();
}

void AMyCharacter::Look(const FInputActionValue& value)
{
	LookInput = value.Get<FVector2D>();
}