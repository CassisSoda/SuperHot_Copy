// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperHot_Copy/Public/HS/Enemy.h"

#include "SHGameMode.h"
#include "Components/SphereComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
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

	// 공격 콜리전 생성
	AttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
	AttackCollision->SetupAttachment(GetMesh(), TEXT("hand_rSocket")); // 오른손에 부착 (소켓 이름 변경 가능)
	AttackCollision->SetSphereRadius(30.0f); // 콜리전 범위 설정
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 기본적으로 비활성화
	AttackCollision->SetCollisionObjectType(ECC_WorldDynamic);
	AttackCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 플레이어와만 충돌

	// 콜리전 이벤트 바인딩
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnAttackHit);

	GeometryCollectionComp = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComp"));
	GeometryCollectionComp->SetupAttachment(RootComponent);
	GeometryCollectionComp->SetVisibility(false); // 기본적으로 숨김
	GeometryCollectionComp->SetSimulatePhysics(false);
	ConstructorHelpers::FObjectFinder<UGeometryCollection>GeoTmp(TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/HS/Fracture/GC_SH_Enemy.GC_SH_Enemy'"));
	if (GeoTmp.Succeeded())
	{
		GeometryCollectionComp->SetRestCollection(GeoTmp.Object);
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
	
	if (GeometryCollectionComp)
	{
		GeometryCollectionComp->SetSimulatePhysics(false);
		GeometryCollectionComp->SetVisibility(false);
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

void AEnemy::OnAttackHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(ASHPlayer::StaticClass())) // 플레이어인지 확인
	{
		ASHPlayer* Player = Cast<ASHPlayer>(OtherActor);
		if (Player)
		{
			// 플레이어에게 데미지를 줌
			UGameplayStatics::ApplyDamage(Player, 10.0f, GetController(), this, UDamageType::StaticClass());
		}
	}
}

void AEnemy::EnableAttackCollision()
{
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}

void AEnemy::DisableAttackCollision()
{
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

