// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent (MeshComp);

	ConstructorHelpers::FObjectFinder<UStaticMeshComponent> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/MW/Assets/Bullet/SM_Bullet.SM_Bullet'"));
	if (TempMesh.Succeeded())
	{
		MeshComp = TempMesh.Object;
	}
	MeshComp->SetRelativeRotation (FRotator(90.f, 0.f, 180.f));

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

