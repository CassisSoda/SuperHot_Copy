// Fill out your copyright notice in the Description page of Project Settings.

#include "HS/Weapons/EnemyBullet.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "SHPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemyBullet::AEnemyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	CapsuleComp = CreateDefaultSubobject <UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	
	CapsuleComp->SetCapsuleHalfHeight(4.f);
	CapsuleComp->SetCapsuleRadius(2.f);

	MeshComp = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MeshComp"));
		MeshComp->SetupAttachment(RootComponent);
		MeshComp->SetRelativeLocation(FVector(7.55f, 0.f, -7.25f));

	ConstructorHelpers::FObjectFinder<UStaticMeshComponent> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/MW/Assets/Bullet/SM_Bullet.SM_Bullet'"));
	if (TempMesh.Succeeded())
	{
		MeshComp = TempMesh.Object;
	}

	// 충돌 프로필을 Overlap으로 설정 (물리적 충격 제거)
	CapsuleComp->SetCollisionProfileName(TEXT("EnemyBullet"));
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 물리 충돌 비활성화, 쿼리만 활성화
	CapsuleComp->SetGenerateOverlapEvents(true); // 오버랩 이벤트 발생 보장
	CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 에너미와 오버랩만 발생
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::OnHit);

}

// Called when the game starts or when spawned
void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();
	// 플레이어 위치 가져오기
	AActor* PlayerRef = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerRef)
	{
		EndPos = PlayerRef->GetActorLocation();
		Destination = (EndPos - GetActorLocation()).GetSafeNormal();
	}

	if (BulletTrailEffect)
	{
		FVector TrailOffset = FVector(-10.f, 0.f, 0.f);
		FRotator TrailRotation = GetActorRotation();

		UGameplayStatics::SpawnEmitterAttached(	
			BulletTrailEffect, MeshComp, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset);

	}
}

// Called every frame
void AEnemyBullet::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);
	SetActorLocation(GetActorLocation() + Destination * Speed * DeltaTime);

}

void AEnemyBullet::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Bullet hit: %s"), *OtherActor->GetName()));
	UE_LOG(LogTemp, Warning, TEXT("Bullet hit: %s"), *OtherActor->GetName());

	ASHPlayer* Player = Cast<ASHPlayer>(OtherActor);
	if (Player)
	{
		UGameplayStatics::ApplyDamage(Player, 50.0f, GetInstigatorController(), this, UDamageType::StaticClass());
		Destroy();
	}
}

