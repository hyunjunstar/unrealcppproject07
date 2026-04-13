// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"

AMyPlayerController::AMyPlayerController()
	:InputMC(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr)
{

}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();


	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMC)
			{
				Subsystem->AddMappingContext(InputMC, 0);
			}
		}
	}
}