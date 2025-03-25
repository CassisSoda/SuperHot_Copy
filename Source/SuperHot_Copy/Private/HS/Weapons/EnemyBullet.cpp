// Fill out your copyright notice in the Description page of Project Settings.

#include "HS/Weapons/EnemyBullet.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyBullet::AEnemyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetRelativeLocationAndRotation(FVector(0.f), FRotator(90.f, 0.f, 180.f));
	SetRootComponent(MeshComp);

	static ConstructorHelpers::FObjectFinder<UStaticMeshComponent> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/MW/Assets/Bullet/SM_Bullet.SM_Bullet'"));
	if (TempMesh.Succeeded())
	{
		MeshComp = TempMesh.Object;
	}

	MeshComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);  
	MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Ignore);

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
	
}

// Called every frame
void AEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(GetActorLocation() + Destination * Speed * DeltaTime);

}

