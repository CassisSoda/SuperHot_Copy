// Fill out your copyright notice in the Description page of Project Settings.


#include "SHEnemy.h"

#include "Bullet.h"
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
	if (!bIsDead && PlayerRef)
	{
		RotateTowardsPlayer();
	}

}

void ASHEnemy::Fire()
{
	if (bIsDead || !BulletClass) return;

	// 발사 위치 가져오기
	FVector FireLocation = FirePoint->GetComponentLocation();

	// 목표 방향 계산
	FVector PlayerLocation = PlayerRef->GetActorLocation();
	FRotator FireRotation = (PlayerLocation - FireLocation).Rotation();

	// 총알 스폰
	GetWorld()->SpawnActor<ABullet>(BulletClass, FireLocation, FireRotation);

	UE_LOG(LogTemp, Warning, TEXT("Projectile Fired!"));

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
		// BP_Gun 인스턴스 생성
		EquippedWeapon = GetWorld()->SpawnActor<AActor>(GunClass);
		if (EquippedWeapon)
		{
			EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_rSocket"));
			EquippedWeapon->SetOwner(this);
		}
	}
	//if (WeaponMesh && SkeletalMesh)
	//{
	//	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	//	WeaponMesh->AttachToComponent(SkeletalMesh, AttachmentRules, FName("hand_rSocket"));
	//}
}

