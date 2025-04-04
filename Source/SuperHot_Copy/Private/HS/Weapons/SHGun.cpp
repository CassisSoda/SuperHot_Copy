// Fill out your copyright notice in the Description page of Project Settings.


#include "HS/Weapons/SHGun.h"
#include "HS/Weapons/Gun.h"
#include "Kismet/GameplayStatics.h"
#include "shDebug.h"
#include "Components/BoxComponent.h"
#include "Bullet.h"

// Sets default values
ASHGun::ASHGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
	FirePoint->SetRelativeLocationAndRotation(FVector(0.000305,-0.237799,-0), FRotator(0,-90,-90));

}

// Called when the game starts or when spawned
void ASHGun::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerRef = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

// Called every frame
void ASHGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASHGun::Fire()	
{
	if (!bCanFire) return;

	// BulletClass 확인 로그 추가
	if (!BulletClass)
	{
		return;
	}
	if (!PlayerRef) 
	{
		return;
	}
	bCanFire = false;
	if (Magazine > 0)
	{

	}
	
	// 발사 위치 및 방향 설정
	FVector MuzzleLocation = FirePoint->GetComponentLocation(); 
	FVector PlayerLocation = PlayerRef->GetActorLocation();
	FRotator MuzzleRotation = (PlayerLocation - MuzzleLocation).Rotation();
	
	if (isPlayerGrabbing)
	{
		FActorSpawnParameters spawnParams;

		if (PlayerRef)
		{
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			spawnParams.bNoFail = true;
			spawnParams.Owner = PlayerRef;
		}
		UE_LOG(LogTemp, Warning, TEXT("[Fire] Player is holding the gun! Spawning PlayerBulletClass."));

		
		//FRotator AimRotate = 
		ABullet* FiredBullet = GetWorld ()->SpawnActor <ABullet>(PlayerBulletClass, MuzzleLocation, MuzzleLocation.Rotation(), spawnParams);
		Debug::NullPrint(FiredBullet, "");
		FiredBullet->SetVelocity (MuzzleLocation.ForwardVector);
		Magazine--;
	}
	else
	{
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
	UE_LOG(LogTemp, Warning, TEXT("Bullet Spawn Location: %s"), *MuzzleLocation.ToString());

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

void ASHGun::SetIsPlayerGrabbing()
{
	isPlayerGrabbing = !isPlayerGrabbing;
}
