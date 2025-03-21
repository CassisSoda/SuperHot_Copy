// Fill out your copyright notice in the Description page of Project Settings.


#include "SHPlayer.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

// Sets default values
ASHPlayer::ASHPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRCamera = CreateDefaultSubobject<UCameraComponent> (TEXT("VRCamera"));
	VRCamera->SetupAttachment (RootComponent);

	ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/MW/Input/IMC_Player.IMC_Player'"));

	if (TempIMC.Succeeded())
	{
		IMC_Player = TempIMC.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempMove(TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/IA_PlayerMove.IA_PlayerMove'"));
	if (TempMove.Succeeded())
	{
		IA_PlayerMove = TempMove.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempTurn(TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/IA_PlayerTurn.IA_PlayerTurn'"));
	if (TempTurn.Succeeded())
	{
		IA_PlayerTurn = TempTurn.Object;
	}
	

}

// Called when the game starts or when spawned
void ASHPlayer::BeginPlay()
{
	Super::BeginPlay();
	GetWorldSettings()->SetTimeDilation(0.005f);
	
}

// Called every frame
void ASHPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASHPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto pc = Cast<APlayerController>(GetController());
	if (pc)
	{
		auto subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Player, 1);
		}
	}

	auto playerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (playerInput)
	{
		playerInput->BindAction(IA_PlayerMove, ETriggerEvent::Triggered, this, &ASHPlayer::Move);
		playerInput->BindAction(IA_PlayerTurn, ETriggerEvent::Triggered, this, &ASHPlayer::Turn);

		playerInput->BindAction(IA_PlayerMove, ETriggerEvent::Started, this, &ASHPlayer::ShiftDilation);
		playerInput->BindAction(IA_PlayerMove, ETriggerEvent::Completed, this, &ASHPlayer::ShiftDilation);
	}

}

void ASHPlayer::Move(const struct FInputActionValue& InputValue)
{
	FVector2D Scale = InputValue.Get<FVector2D>();

	//FVector Direction = VRCamera->GetForwardVector () * Scale.X + VRCamera->GetRightVector () + Scale.Y;
	//AddMovementInput (Direction);

	AddMovementInput (VRCamera->GetForwardVector (), Scale.X);
	AddMovementInput (VRCamera->GetRightVector (), Scale.Y);
}

void ASHPlayer::Turn(const struct FInputActionValue& InputValue)
{
	FVector2D Scale = InputValue.Get<FVector2D>();
	AddControllerPitchInput (Scale.Y);
	AddControllerYawInput (Scale.X);
}

void ASHPlayer::ShiftDilation()
{
	if (isDelay)
	{
		GetWorldSettings()->SetTimeDilation(1.f);
	}
	else
	{
		GetWorldSettings()->SetTimeDilation(0.005f);
	}
	isDelay = !isDelay;
}

