// Fill out your copyright notice in the Description page of Project Settings.


#include "HS/Enemy_Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HS/EnemyFSM.h"

AEnemy_Gun::AEnemy_Gun()
{
	// 부모 클래스(AEnemy)의 SkeletalMeshComponent 사용
	if (GetMesh())
	{
		// 무기 메시 컴포넌트 생성
		GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

		// 총 메시 로드
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Meshes/Jericho_941.Jericho_941'"));
		if (FoundGunMesh.Succeeded())
		{
			GunMesh->SetSkeletalMesh(FoundGunMesh.Object);
		}

		// 부모의 SkeletalMeshComponent의 "hand_rSocket"에 부착
		GunMesh->SetupAttachment(GetMesh(), FName("hand_rSocket"));

		// 위치, 회전 코드
		//GunMesh->SetRelativeLocation(FVector(-15.961346,3.075356,4.126808));
		//GunMesh->SetRelativeRotation(FRotator(-21.094705,-126.115306,18.074879));
//
		ConstructorHelpers::FClassFinder<UAnimInstance>TmpClass(TEXT("/Script/Engine.AnimBlueprint'/Game/HS/Enemy/ABP_Enemy_Gun.ABP_Enemy_Gun'"));
		if (TmpClass.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(TmpClass.Class);
		}
	}
}

void AEnemy_Gun::BeginPlay()
{
	Super::BeginPlay();
}
