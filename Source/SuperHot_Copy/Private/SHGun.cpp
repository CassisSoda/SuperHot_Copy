// Fill out your copyright notice in the Description page of Project Settings.


#include "SHGun.h"

#include "HS/Weapons/Gun.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASHGun::ASHGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;



	// 루트 컴포넌트 설정
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

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

	//// 총구 위치 설정
	//SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	//RootComponent = SceneComponent;
//
	//// 총 메쉬 설정
	//WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	//WeaponMesh->SetupAttachment(RootComponent);
//
	//WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);  // Gun 채널
	//
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh>GunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Meshes/Jericho_941.Jericho_941'"));
	//if (GunMesh.Succeeded())
	//{
	//	WeaponMesh->SetSkeletalMesh(GunMesh.Object);
	//}
//
	//MuzzleOffset = FVector(0.000305f, -0.237799f, 0.f);
}

// Called when the game starts or when spawned
void ASHGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASHGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASHGun::Fire()
{    if (!bCanFire) return;

	bCanFire = false;

	// 발사 위치 및 방향 설정
	FVector MuzzleLocation = FirePoint->GetComponentLocation(); 
	FRotator MuzzleRotation = FirePoint->GetComponentRotation();

	// 발사체 생성
	if (BulletClass)
	{
		GetWorld()->SpawnActor<ABullet>(BulletClass, MuzzleLocation, MuzzleRotation);
	}

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
	GetWorldTimerManager().SetTimer(FireCooldownTimer, this, &ASHGun::ResetFireCooldown, TimeBetweenShots, false);
}

void ASHGun::Collected()
{
	WeaponMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASHGun::Thrown()
{
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ASHGun::ResetFireCooldown()
{
	bCanFire = true;
}
