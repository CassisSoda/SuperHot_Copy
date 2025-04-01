// Fill out your copyright notice in the Description page of Project Settings.


#include "HS/SHEnemy.h"


#include "Engine/DamageEvents.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollection.h"
#include "GeometryCollection/GeometryCollectionExternalRenderInterface.h"
#include "Components/PrimitiveComponent.h"
#include "HS/Enemy.h"
#include "HS/Weapons/EnemyGun.h"
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
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetRelativeLocation(FVector(-2.860987,3.420509,-6.287745));
	WeaponMesh->SetRelativeRotation(FRotator(3.096127,86.838675,17.192416));
	
	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(GetMesh(), TEXT("barrelSocket")); // 손 소켓에 부착

	GeometryCollectionComp = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComp"));
	GeometryCollectionComp->SetupAttachment(RootComponent);
	GeometryCollectionComp->SetVisibility(false);
	GeometryCollectionComp->SetSimulatePhysics(false);
	ConstructorHelpers::FObjectFinder<UGeometryCollection> GeoTmp(TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/HS/Fracture/GC_SH_Enemy.GC_SH_Enemy'"));
	if (GeoTmp.Succeeded())
	{
		GeometryCollectionComp->SetRestCollection(GeoTmp.Object);
		UE_LOG(LogTemp, Log, TEXT("GeometryCollection loaded successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load GeometryCollection! Check the path."));
	}

	// RootComponent 물리 비활성화
	if (UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(RootComponent))
	{
		RootComp->SetSimulatePhysics(false);
		RootComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 기본 충돌은 유지하되 물리 비활성화
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
		Gun = GetWorld()->SpawnActor<AEnemyGun>(GunClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (Gun)
		{

			// Enemy의 SkeletalMesh에 있는 "hand_rSocket"에 부착
			Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_rSocket"));

			UE_LOG(LogTemp, Warning, TEXT("Gun spawned successfully!"));
			
			// 총의 소유자를 Enemy로 설정
			Gun->SetOwner(this);
		}
		        else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to spawn Gun!"));
                }
	}
}




#pragma region 파편화
void ASHEnemy::OnDeath()
{
	// 이미 죽었다면 실행 안 함
	if (bIsDead) return;
	bIsDead = true;  // 사망 상태 플래그 설정

	// SkeletalMesh 숨기기
	GetMesh()->SetVisibility(false);
	//GetMesh()->SetHiddenInGame(true);	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetSimulatePhysics(false);

	// RootComponent 물리 비활성화 및 속도 초기화
	if (UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		RootComp->SetSimulatePhysics(false);
		RootComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RootComp->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
		RootComp->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	}
	
	if (Gun)
	{
		Gun->Destroy();
		Gun = nullptr;
	}
	
	if (GeometryCollectionComp)
	{
		FVector EnemyLocation = GetActorLocation();
		FRotator EnemyRotation = GetActorRotation();
		GeometryCollectionComp->SetWorldLocation(EnemyLocation);
		GeometryCollectionComp->SetWorldRotation(EnemyRotation);
		
		GeometryCollectionComp->SetVisibility(true);
		GeometryCollectionComp->SetSimulatePhysics(true);
		GeometryCollectionComp->SetEnableGravity(true);
		GeometryCollectionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GeometryCollectionComp->SetCollisionObjectType(ECC_PhysicsBody);
		// 초기 속도 초기화 (하늘로 올라가는 문제 방지)
		GeometryCollectionComp->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
		GeometryCollectionComp->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		
		GeometryCollectionComp->ApplyExternalStrain(1000.0f, GetActorLocation(), 150.0f, 1);
		// 방사형 임펄스 적용
		FVector ExplosionCenter = GetActorLocation() + FVector(0.0f, 0.0f, 120.0f); 
		float ExplosionStrength = 1000.0f; // 세기 설정
		float ExplosionRadius = 1200.0f;    // 반경 증가
		GeometryCollectionComp->AddRadialImpulse(ExplosionCenter, ExplosionRadius, ExplosionStrength, ERadialImpulseFalloff::RIF_Linear, true);
		GeometryCollectionComp->AddForce(FVector(0.0f, 0.0f, -8000.0f), NAME_None, true);
		// 일정 시간 후 파편 제거
		GetWorld()->GetTimerManager().SetTimer(DestructionTimerHandle, this, &ASHEnemy::DestroyFragments, 3.0f, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GeometryCollectionComp is NULL!"));
	}
}


float ASHEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead) return 0.0f;

	// 즉사 처리
	bIsDead = true;
	Health = 0.0f;

	// 사망 처리 즉시 호출
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


#pragma endregion