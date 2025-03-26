// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MeshComp"));
//	MeshComp->SetRelativeRotation (FRotator(90.f, 0.f, 180.f));
	//MeshComp->SetRelativeLocationAndRotation (FVector(0.f), FRotator(90.f, 0.f, 180.f));
	MeshComp-> SetRootComponent(MeshComp);

	ConstructorHelpers::FObjectFinder<UStaticMeshComponent> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/MW/Assets/Bullet/SM_Bullet.SM_Bullet'"));
	if (TempMesh.Succeeded())
	{
		MeshComp = TempMesh.Object;
	}
	MeshComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);  // Bullet 채널
	MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Ignore);  // Gun 채널 무시

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	Destination = EndPos - GetActorLocation() ;
	Destination.Normalize();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + Destination * Speed * DeltaTime);
}

