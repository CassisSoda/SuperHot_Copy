// Fill out your copyright notice in the Description page of Project Settings.


#include "HS/Weapons/EnemyGun.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyGun::AEnemyGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject <UBoxComponent>("BoxComp");
	BoxComp->SetBoxExtent(FVector(3.f, 24.f, 15.f));
	SetRootComponent(BoxComp);

	// 총기 메쉬 설정
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);  // Gun 채널

	//  총기 모델 로드
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Meshes/Jericho_941.Jericho_941'"));
	if (GunMesh.Succeeded())
	{
		WeaponMesh->SetSkeletalMesh(GunMesh.Object);
	}
	
	// 총구 위치 설정 (FirePoint 추가)
	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(WeaponMesh, TEXT("barrelSocket")); // "Muzzle" 소켓에 부착
	FirePoint->SetRelativeLocationAndRotation(FVector(0.000305,0.011603,-0), FRotator(0,-90,-90));

}

// Called when the game starts or when spawned
void AEnemyGun::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
}

// Called every frame
void AEnemyGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyGun::Fire()
{
	if (!bCanFire)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot fire: bCanFire is false!"));
		return;
	}
	// BulletClass 확인 로그 추가
	if (!BulletClass)
	{	
		UE_LOG(LogTemp, Error, TEXT("BulletClass is NULL!"));

		return;
	}
	
	if (!PlayerRef)
	{
		PlayerRef = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (!PlayerRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Player ref"))
			return;
		}
	}

	float DistanceToPlayer = FVector::Dist(FirePoint->GetComponentLocation(),PlayerRef->GetActorLocation());

	if (DistanceToPlayer > AttackRange)
	{
		UE_LOG(LogTemp, Warning, TEXT("out of range"))
		return;
	}
	// 발사 위치 및 방향 설정
	FVector MuzzleLocation = FirePoint->GetComponentLocation(); 
	FVector PlayerLocation = PlayerRef->GetActorLocation();
	FRotator MuzzleRotation = (PlayerLocation - MuzzleLocation).Rotation();
	
	// 블루프린트 총알 스폰
	AActor* SpawnedBullet = GetWorld()->SpawnActor<AActor>(BulletClass, MuzzleLocation, MuzzleRotation);
	if (!SpawnedBullet)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn bullet!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Bullet spawned successfully at: %s"), *MuzzleLocation.ToString());
	}
	bCanFire = false;

	// 총구 이펙트 생성
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleEffect, MuzzleLocation, MuzzleRotation);
	}

	// 애니메이션 재생
	if (FireAnimation && WeaponMesh)
	{
		UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(FireAnimation);
		}
	}

	// 일정 시간 후 발사 가능 상태로 변경
	GetWorldTimerManager().SetTimer(FireCooldownTimer, this, &AEnemyGun::ResetFireCooldown, TimeBetweenShots, false);
	UE_LOG(LogTemp, Warning, TEXT("Bullet Spawn Location: %s"), *MuzzleLocation.ToString());

}

void AEnemyGun::ResetFireCooldown()
{
	UE_LOG(LogTemp, Warning, TEXT("ResetFireCooldown() called!"));

	bCanFire = true;
}

