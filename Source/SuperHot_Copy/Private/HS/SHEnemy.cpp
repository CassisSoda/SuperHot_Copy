// Fill out your copyright notice in the Description page of Project Settings.


#include "HS/SHEnemy.h"

#include "HS/Enemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASHEnemy::ASHEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>MestTmp(TEXT("/Script/Engine.SkeletalMesh'/Game/HS/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	if (MestTmp.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MestTmp.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-88), FRotator(0,-90,0));
	}
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetRelativeLocation(FVector(-2.860987,3.420509,-6.287745));
	WeaponMesh->SetRelativeRotation(FRotator(3.096127,86.838675,17.192416));
	
	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(GetMesh(), TEXT("barrelSocket")); // 손 소켓에 부착

}

// Called when the game starts or when spawned
void ASHEnemy::BeginPlay()
{
	Super::BeginPlay();
	AttachWeapon();

	// 플레이어 찾기
	PlayerRef = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// 주기적으로 Fire() 호출
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ASHEnemy::Fire, TimeBetweenShots, true);
	
}

// Called every frame
void ASHEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// PlayerRef가 없으면 다시 찾기
	if (!PlayerRef)
	{
		PlayerRef = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (!PlayerRef)
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerRef NULL"));
			return;
		}
	}

	if (!bIsDead && PlayerRef)
	{
		RotateTowardsPlayer();
	}
	//if (!bIsDead && PlayerRef)
	//{
	//	RotateTowardsPlayer();
	//}

}

void ASHEnemy::Fire()
{
	if (Gun)
	{
		Gun->Fire();  // Gun의 Fire() 호출
		UE_LOG(LogTemp, Warning, TEXT("Enemy fired the gun!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Gun is NULL!"));
	}
}

void ASHEnemy::RotateTowardsPlayer()
{
	if (PlayerRef)
	{
		FVector MyLocation = GetActorLocation();
		FVector PlayerLocation = PlayerRef->GetActorLocation();
		FRotator LookAtRotation = (PlayerLocation - MyLocation).Rotation();

		SetActorRotation(LookAtRotation);
	}
}

void ASHEnemy::AttachWeapon()
{
	if (GunClass)
	{
		// BP_Gun 인스턴스 생성 (위치 지정 추가)
		Gun = GetWorld()->SpawnActor<ASHGun>(GunClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (Gun)
		{
			// 적(Enemy)의 SkeletalMesh에 있는 "hand_rSocket"에 부착
			Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_rSocket"));

			// 총의 소유자를 Enemy로 설정
			Gun->SetOwner(this);

			UE_LOG(LogTemp, Warning, TEXT("Gun successfully attached to enemy."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn Gun!"));
		}
	}
//	if (GunClass)
//	{
//		// BP_Gun 인스턴스 생성
//		Gun = GetWorld()->SpawnActor<ASHGun>(GunClass);
//		if (Gun)
//		{
//			Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_rSocket"));
//			Gun->SetOwner(this);
//		}	
//	}
//
}

