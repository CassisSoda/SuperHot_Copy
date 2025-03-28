// Fill out your copyright notice in the Description page of Project Settings.

#include "HS/Weapons/EnemyBullet.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemyBullet::AEnemyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	CapsuleComp = CreateDefaultSubobject <UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	MeshComp = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetRelativeLocation(FVector(7.55f, 0.f, -7.25f));


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

	//CapsuleComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);  // Bullet 채널
	//CapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Ignore);  // Gun 채널 무시
	CapsuleComp->SetCollisionProfileName(TEXT("Bullet"));


	//// Niagara 컴포넌트 생성
	//BulletTrail = CreateDefaultSubobject<UNiagaraSystem>(TEXT("BulletTrail"));
	//BulletTrail->SetupAttachment(MeshComp);

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

	//	UGameplayStatics::SpawnEmitterAtLocation(
	//GetWorld(), BulletTrailEffect, GetActorLocation(), GetActorRotation(), true);

		UGameplayStatics::SpawnEmitterAttached(
			BulletTrailEffect, MeshComp, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset);
//		UNiagaraFunctionLibrary::SpawnSystemAttached(
//BulletTrailNia, MeshComp, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator,
//EAttachLocation::KeepRelativeOffset, true);
	}
}

// Called every frame
void AEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(GetActorLocation() + Destination * Speed * DeltaTime);

}

