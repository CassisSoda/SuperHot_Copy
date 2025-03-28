// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperHot_Copy/Public/HS/Enemy.h"

#include "SHGameMode.h"
#include "HS/Weapons/WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "SuperHot_Copy/Public/HS/EnemyFSM.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh>MestTmp(TEXT("/Script/Engine.SkeletalMesh'/Game/HS/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	if (MestTmp.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MestTmp.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-88), FRotator(0,-90,0));
	}

	FSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	ConstructorHelpers::FClassFinder<UAnimInstance>TmpClass(TEXT("/Script/Engine.AnimBlueprint'/Game/HS/Enemy/ABP_Enemy_Gun.ABP_Enemy_Gun'"));
	if (TmpClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TmpClass.Class);
	}

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	EquipWeapon();
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (WeaponClass)
	{
		FTransform WeaponTransform;
		WeaponTransform.SetLocation(FVector::ZeroVector);
		WeaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));
	
		Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, WeaponTransform, SpawnParams);
		if (Weapon)
		{
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("hand_rSocket"));
		}
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnemy::EquipWeapon()
{
	if (DefaultWeaponClass)
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->Destroy();
		}

		EquippedWeapon = GetWorld()->SpawnActor<AWeaponBase>(DefaultWeaponClass);
		if (EquippedWeapon)
		{
			EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rSocket"));
		}
	}
}

void AEnemy::FireWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
}

void AEnemy::Die()	// 스테이지
{
	ASHGameMode* GameMode = Cast<ASHGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->OnEnemyKilled();
	}
	Destroy();
}

