// Fill out your copyright notice in the Description page of Project Settings.


#include "HS/SHEnemy.h"


#include "Engine/DamageEvents.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollection.h"
#include "GeometryCollection/GeometryCollectionExternalRenderInterface.h"
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
	
	//SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	//SkeletalMesh->SetupAttachment(RootComponent);
		
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetRelativeLocation(FVector(-2.860987,3.420509,-6.287745));
	WeaponMesh->SetRelativeRotation(FRotator(3.096127,86.838675,17.192416));
	
	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(GetMesh(), TEXT("barrelSocket")); // 손 소켓에 부착
	
	GeometryCollectionComp = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComp"));
	GeometryCollectionComp->SetupAttachment(RootComponent);
	GeometryCollectionComp->SetVisibility(false); // 기본적으로 숨김
	GeometryCollectionComp->SetSimulatePhysics(false);
	//GeometryCollectionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UGeometryCollection>GeoTmp(TEXT("/Script/Engine.SkeletalMesh'/Game/HS/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	if (GeoTmp.Succeeded())
	{
		GeometryCollectionComp->SetRestCollection(GeoTmp.Object);
	}
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
	
	if (GeometryCollectionComp)
	{
		GeometryCollectionComp->SetSimulatePhysics(false);
		GeometryCollectionComp->SetVisibility(false);
	}
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
			return;
		}
	}

	if (!bIsDead && PlayerRef)
	{
		RotateTowardsPlayer();
	}

}

void ASHEnemy::Fire()
{
	if (Gun)
	{
		Gun->Fire();  // Gun의 Fire() 호출
		UE_LOG(LogTemp, Warning, TEXT("Enemy fired the gun!"));
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
		}
	}
}




#pragma region 파편화
void ASHEnemy::OnDeath()
{
	// 이미 죽었다면 실행 안 함
	if (bIsDead) return;
	bIsDead = true;  // 사망 상태 플래그 설정

	// 스켈레탈 메시 숨기기 및 충돌 제거
	GeometryCollectionComp->SetVisibility(false);
	GeometryCollectionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (GeometryCollectionComp)
	{
		GeometryCollectionComp->SetVisibility(true);
		GeometryCollectionComp->SetSimulatePhysics(true);

		GeometryCollectionComp->SetEnableGravity(true);
		GeometryCollectionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	// 일정 시간 후 파편 제거
	GetWorld()->GetTimerManager().SetTimer(DestructionTimerHandle, this, &ASHEnemy::DestroyFragments, 5.0f, false);
}


float ASHEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead) return 0.0f; // 이미 죽었으면 처리 안 함

	// 즉사
	bIsDead = true;  
	Health = 0.0f;  

	// 사망 처리 함수 호출
	OnDeath();  

	return DamageAmount; 
}

void ASHEnemy::DestroyFragments()
{
		if (GeometryCollectionComp) // 파편 존재하면
		{
			GeometryCollectionComp->DestroyComponent(); //제거
		}
		Destroy(); // 적 액터 제거
}

void ASHEnemy::DebugTakeDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy took damage!"));
    
	// 50의 데미지 적용 (한 방에 죽게 하려면 100 이상 설정)
	TakeDamage(100.0f, FDamageEvent(), nullptr, nullptr);
}

#pragma endregion