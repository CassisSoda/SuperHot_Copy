// Fill out your copyright notice in the Description page of Project Settings.


#include "HS/Enemy_Bat.h"

AEnemy_Bat::AEnemy_Bat()
{
	// 부모 클래스(AEnemy)의 SkeletalMeshComponent 사용
	if (GetMesh())
	{
		// 무기 메시 컴포넌트 생성
		BatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

		// bat 메시 로드
		static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundBatMesh(TEXT("/Script/Engine.StaticMesh'/Game/HS/assets/Bat/source/BaseballBat_Low.BaseballBat_Low'"));
		if (FoundBatMesh.Succeeded())
		{
			BatMesh->SetStaticMesh(FoundBatMesh.Object);
		}

		// 부모의 SkeletalMeshComponent의 "hand_rSocket"에 부착
		BatMesh->SetupAttachment(GetMesh(), FName("hand_rSocket"));

		// 위치, 회전 코드
		BatMesh->SetRelativeLocation(FVector(-10.515486,2.878071,31.233261));
		BatMesh->SetRelativeRotation(FRotator(89.772584,24.949097,179.980674));
	}
}

void AEnemy_Bat::BeginPlay()
{
	Super::BeginPlay();
}
