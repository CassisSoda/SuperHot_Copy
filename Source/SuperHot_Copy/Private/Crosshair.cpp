// Fill out your copyright notice in the Description page of Project Settings.


#include "Crosshair.h"
#include "shDebug.h"

// Sets default values
ACrosshair::ACrosshair()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent> (TEXT("SceneComp"));
	SetRootComponent (SceneComp);

	DotPlane = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Dot_Plane"));
	DotPlane->SetupAttachment (RootComponent);
	DotPlane->SetCollisionEnabled (ECollisionEnabled::NoCollision);
	DotPlane->SetRelativeRotation (FRotator(90.f, 0.f, 0.f));
	DotPlane->SetRelativeScale3D (FVector(0.01f));
	
	CrossPlane = CreateDefaultSubobject<UStaticMeshComponent> (TEXT("Cross_Plane"));
	CrossPlane->SetupAttachment (RootComponent);
	CrossPlane->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CrossPlane->SetRelativeRotation (FRotator(45.f, 90.f, 90.f));
	CrossPlane->SetRelativeScale3D (FVector(0.01f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempPlane(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (TempPlane.Succeeded())
	{
		DotPlane->SetStaticMesh (TempPlane.Object);
		CrossPlane->SetStaticMesh(TempPlane.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> DotMat(TEXT("/Script/Engine.Material'/Game/MW/Assets/Crosshair/M_dot.M_dot'"));
	if (DotMat.Succeeded())
	{
		DotPlane->SetMaterial (0,DotMat.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterial> CrossMat(TEXT("/Script/Engine.Material'/Game/MW/Assets/Crosshair/M_Crosshair.M_Crosshair'"));
	if (CrossMat.Succeeded())
	{
		CrossPlane->SetMaterial(0,CrossMat.Object);
	}
	

}

// Called when the game starts or when spawned
void ACrosshair::BeginPlay()
{
	Super::BeginPlay();
	GetComponents <UStaticMeshComponent>(childrenComp);
	Debug::Print(FString::Printf(TEXT("%d"), childrenComp.Num()));
	
}

// Called every frame
void ACrosshair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACrosshair::ChangeType()
{
	switch (Chtype)
	{
		case CrosshairType:: dotType	:	{ DrawDot ();	}	break;
		case CrosshairType:: crossType	:	{ DrawCross ();	}	break;
	}
}

void ACrosshair::DrawDot()
{
	for (auto* child : childrenComp)
	{
		if (child)
		{
			if (child->GetName().Contains(TEXT("Dot")))
			{
				child->SetHiddenInGame(false);
			}
			else
			{
				child->SetHiddenInGame(true);
			}
		}
	}
	
}

void ACrosshair::DrawCross()
{
	//for (UStaticMeshComponent* child : RootComponent->GetChildrenComponents(false))
	//{
	//	if (child->GetName().Contains("Cross"))
	//	{
	//		child->SetHiddenInGame(false);
	//	}
	//	else
	//	{
	//		child->SetHiddenInGame(true);
	//	}
	//}
	//UStaticMeshComponent* targetcomp = this->GetComponentByClass (UStaticMeshComponent::StaticClass ());
}

