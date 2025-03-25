// Fill out your copyright notice in the Description page of Project Settings.


#include "SHPlayer.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "MotionControllerComponent.h"
#include "Engine/SkeletalMesh.h"
#include "shDebug.h"
#include "Components/SphereComponent.h"
#include "HandAnimInstance.h"


// Sets default values
ASHPlayer::ASHPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRCamera = CreateDefaultSubobject<UCameraComponent> (TEXT("VRCamera"));
	VRCamera->SetupAttachment (RootComponent);

	LeftHand = CreateDefaultSubobject <UMotionControllerComponent>(	TEXT("LeftHand"));
	LeftHand->SetupAttachment (RootComponent);

	LeftHandMesh = CreateDefaultSubobject <USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	LeftHandMesh->SetupAttachment (LeftHand);

	LeftHandCollision = CreateDefaultSubobject<USphereComponent> (TEXT("LeftHandCollision"));
	LeftHandCollision->SetupAttachment (LeftHand);

	RightHand = CreateDefaultSubobject <UMotionControllerComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment (RootComponent);

	RightHandMesh = CreateDefaultSubobject <USkeletalMeshComponent>(TEXT("RightHandMesh"));
	RightHandMesh->SetupAttachment (RightHand);

	RightHandCollision = CreateDefaultSubobject <USphereComponent>(TEXT("RightHandCollision"));
	RightHandCollision->SetupAttachment(RightHand);



	#pragma region Input_Constructor
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

	//Right Button
	ConstructorHelpers::FObjectFinder<UInputAction> TempB(TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/Punch/IA_PunchB.IA_PunchB'"));
	if (TempB.Succeeded())
	{
		IA_BPunch = TempB.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempRGrip(TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/Punch/IA_RGrip.IA_RGrip'"));
	if (TempB.Succeeded())
	{
		IA_RGrip = TempRGrip.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempRTrigger(TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/Punch/IA_RTrigger.IA_RTrigger'"));
	if (TempRTrigger.Succeeded())
	{
		IA_RTrigger = TempRTrigger.Object;
	}

	//Left Button
	ConstructorHelpers::FObjectFinder<UInputAction> TempY(TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/Punch/IA_PunchY.IA_PunchY'"));
	if (TempY.Succeeded())
	{
		IA_YPunch = TempY.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempLTrigger(TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/Punch/IA_LTrigger.IA_LTrigger'"));
	if (TempLTrigger.Succeeded())
	{
		IA_LTrigger = TempLTrigger.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempLGrip(TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/Punch/IA_LGrip.IA_LGrip'"));
	if (TempLGrip.Succeeded())
	{
		IA_LGrip = TempLGrip.Object;
	}


#pragma endregion Input_Constructor

	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempLeft(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
	if (TempLeft.Succeeded())
	{
		LeftHandMesh->SetSkeletalMesh(TempLeft.Object);

	}
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempRight(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
	if (TempRight.Succeeded())
	{
		RightHandMesh->SetSkeletalMesh (TempRight.Object);
	}
	ConstructorHelpers::FClassFinder<UAnimInstance> TempLeftAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/MW/Blueprints/ABP_Left.ABP_Left_C'"));
	if (TempLeftAnim.Succeeded ())
	{
		LeftHandMesh->SetAnimInstanceClass(TempLeftAnim.Class);
	}
	ConstructorHelpers::FClassFinder<UAnimInstance> TempRightAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/MW/Blueprints/ABP_Right.ABP_Right_C'"));
	if (TempRightAnim.Succeeded())
	{
		RightHandMesh->SetAnimInstanceClass(TempRightAnim.Class);
	}


	LeftHandMesh->SetRelativeRotation(FRotator(-85.f, -180.f, 90.f));
	RightHandMesh->SetRelativeRotation (FRotator(85.f, 0.f, 90.f));

	LeftHandCollision->SetRelativeLocation (FVector(7.5f, 0.f, 0.f));
	LeftHandCollision->SetRelativeScale3D (FVector(0.4f));
	LeftHandCollision->SetCollisionEnabled (ECollisionEnabled::NoCollision);

	RightHandCollision->SetRelativeLocation(FVector(7.5f, 0.f, 0.f));
	RightHandCollision->SetRelativeScale3D (FVector(0.4f));
	RightHandCollision->SetCollisionEnabled (ECollisionEnabled::NoCollision);


}

// Called when the game starts or when spawned
void ASHPlayer::BeginPlay()
{
	Super::BeginPlay();

	RightAnim = Cast<UHandAnimInstance>(RightHandMesh->GetAnimInstance ());
	LeftAnim = Cast<UHandAnimInstance>(LeftHandMesh->GetAnimInstance());

	GetWorldSettings()->SetTimeDilation(0.005f);
	
}

// Called every frame
void ASHPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bRightPunch = isBPressed && isRGripPressed && isRTriggerPressed;
	if (!isGrapping && bRightPunch && RightPressedKeys.Contains (EKeys::Q) &&
		RightPressedKeys.Contains (EKeys::E) && RightPressedKeys.Contains (EKeys::C))
	{
		RightPunch ();
	}
	bLeftPunch = isYPressed && isLGripPressed && isLTriggerPressed;
	if (!isGrapping && bLeftPunch && LeftPressedKeys.Contains(EKeys::I) &&
		LeftPressedKeys.Contains(EKeys::P) && LeftPressedKeys.Contains(EKeys::M))
	{
		LeftPunch ();
	}
	
	if(isGrapping)
		RightAnim->GripAlpha = 1.f;
	

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

		playerInput->BindAction(IA_RGrip, ETriggerEvent::Started, this, &ASHPlayer::DrawGrapStraight);
		
		#pragma region Punch_Bind
		playerInput->BindAction(IA_BPunch, ETriggerEvent::Triggered, this, &ASHPlayer::OnBPressed);
		playerInput->BindAction(IA_BPunch, ETriggerEvent::Completed,this, &ASHPlayer::OnBReleased );
		
		playerInput->BindAction(IA_RTrigger, ETriggerEvent::Triggered,this, &ASHPlayer::OnRTriggerPressed);
		playerInput->BindAction(IA_RTrigger, ETriggerEvent::Completed,this, &ASHPlayer::OnRTriggerReleased );
		
		playerInput->BindAction(IA_RGrip, ETriggerEvent::Triggered, this, &ASHPlayer::OnRGripPressed);
		playerInput->BindAction(IA_RGrip, ETriggerEvent::Completed, this, &ASHPlayer::OnRGripReleased );

		playerInput->BindAction(IA_YPunch, ETriggerEvent::Triggered, this, &ASHPlayer::OnYPressed);
		playerInput->BindAction(IA_YPunch, ETriggerEvent::Completed, this, &ASHPlayer::OnYReleased);

		playerInput->BindAction(IA_LTrigger, ETriggerEvent::Triggered, this, &ASHPlayer::OnLTriggerPressed);
		playerInput->BindAction(IA_LTrigger, ETriggerEvent::Completed, this, &ASHPlayer::OnLTriggerReleased);

		playerInput->BindAction(IA_LGrip, ETriggerEvent::Triggered, this, &ASHPlayer::OnLGripPressed);
		playerInput->BindAction(IA_LGrip, ETriggerEvent::Completed, this, &ASHPlayer::OnLGripReleased);
		#pragma endregion Punch_Bind
		
		
		
	}

}

void ASHPlayer::Move(const struct FInputActionValue& InputValue)
{
	FVector2D Scale = InputValue.Get<FVector2D>(); 

	AddMovementInput (VRCamera->GetForwardVector (), Scale.X);
	AddMovementInput (VRCamera->GetRightVector (), Scale.Y);

}

void ASHPlayer::Turn(const struct FInputActionValue& InputValue)
{
	FVector2D Scale = InputValue.Get<FVector2D>();
	AddControllerPitchInput (Scale.Y);
	AddControllerYawInput (Scale.X);
}

void ASHPlayer::OnBPressed(const FInputActionValue& InputValue)
{
	if (!RightPressedKeys.Contains(EKeys::Q))
	{
		isBPressed = true;
		RightPressedKeys.Add(EKeys::Q);
	}

}

void ASHPlayer::OnBReleased(const FInputActionValue& InputValue)
{
	if (RightPressedKeys.Contains(EKeys::Q))
	{
		RightPressedKeys.RemoveSingle(EKeys::Q);
		isBPressed = false;
		isDelay = false;
	}
	ShiftDilation ();	
}

void ASHPlayer::OnRTriggerPressed(const FInputActionValue& InputValue)
{	
	RightAnim->TriggerAlpha = InputValue.Get <float>();
	if (!RightPressedKeys.Contains(EKeys::E) && !isGrapping)
	{
		isRTriggerPressed = true;
		RightPressedKeys.Add(EKeys::E);

	}
}

void ASHPlayer::OnRTriggerReleased(const FInputActionValue& InputValue)
{
	RightAnim->TriggerAlpha = 0.f;
	if (RightPressedKeys.Contains(EKeys::E))
	{
		RightPressedKeys.RemoveSingle(EKeys::E);
		isRTriggerPressed = false;
		isDelay = false;
	}
	ShiftDilation ();
}

void ASHPlayer::OnRGripPressed(const FInputActionValue& InputValue)
{
	RightAnim->GripAlpha = InputValue.Get <float>();
	if (!RightPressedKeys.Contains(EKeys::C))
	{
		isRGripPressed = true;
		RightPressedKeys.Add (EKeys::C);
	}
}

void ASHPlayer::OnRGripReleased(const FInputActionValue& InputValue)
{
	RightAnim->GripAlpha = 0.f;

	if (RightPressedKeys.Contains(EKeys::C))
	{
		RightPressedKeys.RemoveSingle(EKeys::C);
		isRGripPressed = false;
		isDelay = false;
	}
	ShiftDilation ();	
}

void ASHPlayer::RightPunch()
{
	isDelay = true;
	ShiftDilation();
	Debug::Print ("RightPunch");
	
	UE_LOG (LogTemp, Log, TEXT("RightPunch"));
}

void ASHPlayer::ResetRightCombo()
{
	isBPressed = false;
	isRTriggerPressed = false;
	isRGripPressed = false;

	RightPressedKeys.Empty();

}

void ASHPlayer::OnYPressed(const FInputActionValue& InputValue)
{
	if (!LeftPressedKeys.Contains(EKeys::I))
	{
		isYPressed = true;
		LeftPressedKeys.Add(EKeys::I);
	}
}

void ASHPlayer::OnYReleased(const FInputActionValue& InputValue)
{
	if (LeftPressedKeys.Contains(EKeys::I))
	{
		LeftPressedKeys.RemoveSingle(EKeys::I);
		isYPressed = false;
		isDelay = false;
	}
	ShiftDilation();
}

void ASHPlayer::OnLTriggerPressed(const FInputActionValue& InputValue)
{
	LeftAnim->TriggerAlpha = InputValue.Get <float>();
	if (!LeftPressedKeys.Contains(EKeys::P))
	{
		isLTriggerPressed = true;
		LeftPressedKeys.Add(EKeys::P);
	}
}

void ASHPlayer::OnLTriggerReleased(const FInputActionValue& InputValue)
{
	LeftAnim->TriggerAlpha = 0.f;
	if (LeftPressedKeys.Contains(EKeys::P))
	{
		LeftPressedKeys.RemoveSingle(EKeys::P);
		isLTriggerPressed = false;
		isDelay = false;
	}
	ShiftDilation();
}

void ASHPlayer::OnLGripPressed(const FInputActionValue& InputValue)
{
	LeftAnim->GripAlpha = InputValue.Get <float>();
	if (!LeftPressedKeys.Contains(EKeys::M))
	{
		isLGripPressed = true;
		LeftPressedKeys.Add(EKeys::M);
	}
}

void ASHPlayer::OnLGripReleased(const FInputActionValue& InputValue)
{
	LeftAnim->GripAlpha = 0.f;
	if (LeftPressedKeys.Contains(EKeys::M))
	{
		LeftPressedKeys.RemoveSingle(EKeys::M);
		isLGripPressed = false;
		isDelay = false;
	}
	ShiftDilation();
}


void ASHPlayer::LeftPunch()
{
	isDelay = true;
	ShiftDilation();
	Debug::Print("LeftPunch");
	UE_LOG(LogTemp, Log, TEXT("LeftPunch"));
}

void ASHPlayer::ResetLeftCombo()
{
	isBPressed = false;
	isRTriggerPressed = false;
	isRGripPressed = false;

	RightPressedKeys.Empty();
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
		//CustomTimeDilation = 200.f;
		
	}
	isDelay = !isDelay;

	if (bRightPunch)
	{
		RightHandCollision->SetCollisionEnabled (ECollisionEnabled::QueryOnly);
		RightAnim->isPunching = true;
	}
	else
	{
		RightHandCollision->SetCollisionEnabled (ECollisionEnabled::NoCollision);
		RightAnim->isPunching = false;
	}

	if (bLeftPunch)
	{
		LeftHandCollision->SetCollisionEnabled (ECollisionEnabled::QueryOnly);
		LeftAnim->isPunching = true;
	}
	else
	{
		LeftHandCollision->SetCollisionEnabled (ECollisionEnabled::NoCollision);
		LeftAnim->isPunching = false;
	}
}

//TODO
void ASHPlayer::OnEnemyOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void ASHPlayer::DrawGrapStraight()
{
	FVector StartPos = VRCamera->GetComponentLocation ();
	FVector EndPos = StartPos + VRCamera->GetForwardVector () * 150.f;

	FHitResult result;
	FCollisionQueryParams params;
	params.AddIgnoredActor (this);
	bool bHit = GetWorld()->LineTraceSingleByChannel (result, StartPos, EndPos, ECC_Visibility, params);


	if (bHit && result.GetActor()->GetActorNameOrLabel().Contains("Gun"))
	{
		isGrapping = true;
		GrapActor (result.GetActor ());
		isDelay = true; 
		ShiftDilation ();
		
	}
}

void ASHPlayer::GrapActor(AActor* actor)
{
	FName WeaponSocket(TEXT("weapon_Socket"));
	if (RightHandMesh->DoesSocketExist(WeaponSocket))
	{
		actor->AttachToComponent (RightHandMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
	}
}

