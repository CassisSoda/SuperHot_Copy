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
#include "Engine/OverlapResult.h"
#include "HS/Weapons/SHGun.h"
#include "Components/ChildActorComponent.h"
#include "Crosshair.h"
#include "SHGameMode.h"
#include "Kismet/GameplayStatics.h"


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

	RightHandAim = CreateDefaultSubobject <UMotionControllerComponent>(TEXT("RightHandAim"));
	RightHandAim->SetupAttachment(RootComponent);

	RightHandMesh = CreateDefaultSubobject <USkeletalMeshComponent>(TEXT("RightHandMesh"));
	RightHandMesh->SetupAttachment (RightHand);

	RightHandCollision = CreateDefaultSubobject <USphereComponent>(TEXT("RightHandCollision"));
	RightHandCollision->SetupAttachment(RightHand);

	CrosshairComp = CreateDefaultSubobject <UChildActorComponent>(TEXT("CrosshairComp"));
	CrosshairComp->SetupAttachment (RootComponent);



	#pragma region InputAction_Constructor
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

	ConstructorHelpers::FObjectFinder<UInputAction> TempRespawn(TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/IA_RespawnPlayer.IA_RespawnPlayer'"));
	if (TempRespawn.Succeeded())
	{
		IA_RespawnPlayer = TempRespawn.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempKill(TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/IA_TestKill.IA_TestKill'"));
	if (TempKill.Succeeded())
	{
		IA_TestKill = TempKill.Object;
	}



#pragma endregion InputAction_Constructor

	
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
	ConstructorHelpers::FClassFinder<AActor> TempCrosshair(TEXT("/Script/Engine.Blueprint'/Game/MW/Blueprints/BP_Crosshair.BP_Crosshair_C'"));
	if (TempCrosshair.Succeeded())
	{
		CrosshairComp->SetChildActorClass (TempCrosshair.Class);
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

	shGameMode = Cast<ASHGameMode>(GetWorld()->GetAuthGameMode ());
	
}

// Called every frame
void ASHPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawCrosshair ();

	bRightPunch = isBPressed && isRGripPressed && isRTriggerPressed;
	if (!isGrabbing && bRightPunch && RightPressedKeys.Contains (EKeys::Q) &&
		RightPressedKeys.Contains (EKeys::E) && RightPressedKeys.Contains (EKeys::C))
	{
		RightPunch ();
	}
	bLeftPunch = isYPressed && isLGripPressed && isLTriggerPressed;
	if (!isGrabbing && bLeftPunch && LeftPressedKeys.Contains(EKeys::I) &&
		LeftPressedKeys.Contains(EKeys::P) && LeftPressedKeys.Contains(EKeys::M))
	{
		LeftPunch ();
	}
	
	if(isGrabbing)
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

		playerInput->BindAction(IA_RGrip, ETriggerEvent::Started, this, &ASHPlayer::TryGrab);
		playerInput->BindAction(IA_RGrip, ETriggerEvent::Completed, this, &ASHPlayer::TryRelease);
		
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

		playerInput->BindAction(IA_RTrigger, ETriggerEvent::Started, this, &ASHPlayer::GunFire);
		playerInput->BindAction(IA_TestKill, ETriggerEvent::Started, this, &ASHPlayer::TestKill);
		playerInput->BindAction(IA_RespawnPlayer, ETriggerEvent::Started, this, &ASHPlayer::ClearPlayerState);
		
		
	}

}

void ASHPlayer::Move(const struct FInputActionValue& InputValue)
{
	if(isDead) return;
	FVector2D Scale = InputValue.Get<FVector2D>(); 

	AddMovementInput (VRCamera->GetForwardVector (), Scale.X);
	AddMovementInput (VRCamera->GetRightVector (), Scale.Y);

}

void ASHPlayer::Turn(const struct FInputActionValue& InputValue)
{
	if (isDead) return;
	FVector2D Scale = InputValue.Get<FVector2D>();
	AddControllerPitchInput (Scale.Y);
	AddControllerYawInput (Scale.X);
}

void ASHPlayer::OnBPressed(const FInputActionValue& InputValue)
{
	if (isDead) return;
	if (!RightPressedKeys.Contains(EKeys::Q))
	{
		isBPressed = true;
		RightPressedKeys.Add(EKeys::Q);
	}

}

void ASHPlayer::OnBReleased(const FInputActionValue& InputValue)
{
	if (isDead) return;
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
	if (isDead) return;
	RightAnim->TriggerAlpha = InputValue.Get <float>();
	if (!RightPressedKeys.Contains(EKeys::E) && !isGrabbing)
	{
		isRTriggerPressed = true;
		RightPressedKeys.Add(EKeys::E);

	}
}

void ASHPlayer::OnRTriggerReleased(const FInputActionValue& InputValue)
{
	if (isDead) return;
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
	if (isDead) return;
	RightAnim->GripAlpha = InputValue.Get <float>();
	if (!RightPressedKeys.Contains(EKeys::C))
	{
		isRGripPressed = true;
		RightPressedKeys.Add (EKeys::C);
	}
}

void ASHPlayer::OnRGripReleased(const FInputActionValue& InputValue)
{
	if (isDead) return;
	
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
	bRightPunch = false;

	RightPressedKeys.Empty();

}

void ASHPlayer::OnYPressed(const FInputActionValue& InputValue)
{
	if (isDead) return;
	
	if (!LeftPressedKeys.Contains(EKeys::I))
	{
		isYPressed = true;
		LeftPressedKeys.Add(EKeys::I);
	}
}

void ASHPlayer::OnYReleased(const FInputActionValue& InputValue)
{
	if (isDead) return;
	
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
	if (isDead) return;
	LeftAnim->TriggerAlpha = InputValue.Get <float>();
	if (!LeftPressedKeys.Contains(EKeys::P))
	{
		isLTriggerPressed = true;
		LeftPressedKeys.Add(EKeys::P);
	}
}

void ASHPlayer::OnLTriggerReleased(const FInputActionValue& InputValue)
{
	if (isDead) return;
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
	if (isDead) return;
	LeftAnim->GripAlpha = InputValue.Get <float>();
	if (!LeftPressedKeys.Contains(EKeys::M))
	{
		isLGripPressed = true;
		LeftPressedKeys.Add(EKeys::M);
	}
}

void ASHPlayer::OnLGripReleased(const FInputActionValue& InputValue)
{
	if (isDead) return;                                                     
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
	isYPressed = false;
	isLTriggerPressed = false;
	isLGripPressed = false;
	bLeftPunch = false;

	LeftPressedKeys.Empty();
}

void ASHPlayer::GunFire(const struct FInputActionValue& InputValue)
{
	if(!isGrabbing || isDead)
		return;
	Debug::Print (FString("Trigger!"));
	
	auto gun = Cast<ASHGun>(GrabObject);
	//auto gun1 = Cast<ASHGun>(GrabObjectComp);
	//Debug::NullPrint(gun1, FString(""));


	if (gun)
	{
		Debug::Print(gun->GetActorNameOrLabel ());
		gun->Fire();

	}

}

void ASHPlayer::TestKill()
{
	playerHP = 0; 
	isDead = true;
	Debug::Print("Test Kill!!!");
}
void ASHPlayer::ShiftDilation()
{
	if(isDead) return;

	if (isDelay)
	{
		GetWorldSettings()->SetTimeDilation(1.f);
	}
	else
	{
		GetWorldSettings()->SetTimeDilation(0.005f);
		
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

void ASHPlayer::TryGrab()
{
	if(isGrabbing || isDead)
		return;
	FVector HandPos = RightHand->GetComponentLocation ();
	TArray<FOverlapResult> HitObjects;

	FCollisionQueryParams params;
	params.AddIgnoredActor (this);
	bool bHit = GetWorld()->OverlapMultiByChannel (HitObjects, HandPos, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere (GrabRadius), params);
	DrawSphere (GetWorld (),HandPos,1.f,FColor::Yellow,GrabRadius);

	if(!bHit)
		return;
	
	int Nearest = 0;

	for (int i = 0; i < HitObjects.Num(); i++)
	{
		auto HitComp = HitObjects[i].GetComponent ();
		if (!HitComp->IsSimulatingPhysics())
			continue;

		isGrabbing = true;

		FVector NearestPos = HitObjects[Nearest].GetActor ()->GetActorLocation ();
		float NearestDistance = FVector::Distance (NearestPos, HandPos);

		FVector NextPos = HitObjects[i].GetActor()->GetActorLocation();
		float NextDistance = FVector::Distance(NextPos, HandPos);

		if (NextDistance < NearestDistance)
		{
			Nearest = i;
		}

	}
	FString s = HitObjects[Nearest].GetActor()->GetActorNameOrLabel();
	Debug::Print(s);

	if (isGrabbing && HitObjects[Nearest].GetActor()->GetActorNameOrLabel().Contains("SHGun"))
	{
		GrabObject = HitObjects[Nearest].GetActor();
		GrabObjectComp = HitObjects[Nearest].GetComponent ();

		GrabObjectComp->SetSimulatePhysics (false);
		GrabObjectComp->SetCollisionEnabled (ECollisionEnabled::NoCollision);
		GrabActor (GrabObject);
		//GrabActor (HitObjects[Nearest].GetActor ());
		isDelay = true;
		ShiftDilation ();
		
	}
	
}

void ASHPlayer::GrabActor(AActor* actor)
{
	FName WeaponSocket(TEXT("weapon_Socket"));
	if (RightHandMesh->DoesSocketExist(WeaponSocket))
	{
		actor->AttachToComponent (RightHandMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);

		ACrosshair* ChildCross = Cast<ACrosshair>(CrosshairComp->GetChildActor ());
		if (ChildCross)
		{
			ChildCross->SetChType (CrosshairType::crossType);
		}

		ASHGun* gun = Cast<ASHGun>(GrabObject);
		if (gun)
		{
			gun->SetIsPlayerGrabbing ();
		}
	}
}

void ASHPlayer::TryRelease()
{
	if(!isGrabbing || isDead)
		return;

	GrabObjectComp->DetachFromComponent (FDetachmentTransformRules::KeepWorldTransform);
	GrabObjectComp->SetSimulatePhysics (true);
	GrabObjectComp->SetCollisionEnabled (ECollisionEnabled::QueryAndPhysics);
	isGrabbing = false;

	ACrosshair* ChildCross = Cast<ACrosshair>(CrosshairComp->GetChildActor());
	if (ChildCross)
	{
		ChildCross->SetChType(CrosshairType::dotType);
	}
}

void ASHPlayer::DrawCrosshair()
{
	FVector StartPos = RightHandAim->GetComponentLocation ();
	FVector EndPos = StartPos + RightHandAim->GetForwardVector () * 10000;
	FHitResult HitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor (this);

	bool bHit = GetWorld ()->LineTraceSingleByChannel (HitInfo, StartPos,EndPos, ECC_Visibility, params);
	float dist = 0.f;

	if (bHit)
	{
		CrosshairComp->SetWorldLocation (HitInfo.Location);
		dist = FVector::Distance(VRCamera->GetComponentLocation (), HitInfo.Location);
	}
	else
	{
		CrosshairComp->SetWorldLocation (EndPos);
		dist = FVector::Distance(VRCamera->GetComponentLocation(), EndPos);
	}

	dist = FMath::Max (1, dist);

	CrosshairComp->SetWorldScale3D (FVector(dist));

	FVector DirectionCrosshair = CrosshairComp->GetComponentLocation () - VRCamera->GetComponentLocation ();
	CrosshairComp->SetWorldRotation (FRotationMatrix::MakeFromX(DirectionCrosshair).Rotator());

}

float ASHPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	playerHP = 0.f;
	isDead = true;
	
	Debug::Print("Player Die!!!!!!");
	Debug::Print("Press R to Restart!!!");
	return playerHP;
}

void ASHPlayer::ClearPlayerState()
{
	if (!isDead) return;

	if (isGrabbing && GrabObject && GrabObjectComp)
	{	
		GrabObjectComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		GrabObjectComp->SetSimulatePhysics(true);
		GrabObjectComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		ACrosshair* ChildCross = Cast<ACrosshair>(CrosshairComp->GetChildActor());
		if (ChildCross)
		{
			ChildCross->SetChType(CrosshairType::dotType);
		}

		GrabObject = nullptr;
		GrabObjectComp = nullptr;
	}

	#pragma region State_Reset
	isDelay = true;
	
	ResetRightCombo ();
	ResetLeftCombo ();
	
	isGrabbing = false;
	isDead = false;

	#pragma endregion State_Reset
	
	shGameMode = Cast<ASHGameMode>(GetWorld()->GetAuthGameMode());
	Debug::NullPrint(shGameMode, "");
	if (shGameMode)
	{
		Debug::Print("RESPAWN Request???");
		shGameMode->StageReLoad();
	}
	
}

