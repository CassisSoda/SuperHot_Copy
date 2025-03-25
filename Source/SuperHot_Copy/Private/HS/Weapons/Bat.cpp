// Fill out your copyright notice in the Description page of Project Settings.


#include "HS/Weapons/Bat.h"


ABat::ABat()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>BatTmpMesh(TEXT("/Script/Engine.StaticMesh'/Game/HS/assets/Bat/source/BaseballBat_Low.BaseballBat_Low'"));
	if(BatTmpMesh.Succeeded())
	{
		BatMesh->SetStaticMesh(BatTmpMesh.Object);
	}
	
}

void ABat::Fire_Implementation()
{
	FHitResult HitResult;
	FVector Start = MeshComp->GetSocketLocation("Start");
	FVector End = Start + MeshComp->GetForwardVector() * Range;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams))
	{
		AActor* HitActor = HitResult.GetActor();	// 데미지 적용 로직
	}
}

