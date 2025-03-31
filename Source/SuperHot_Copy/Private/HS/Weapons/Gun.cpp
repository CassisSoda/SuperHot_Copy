// Fill out your copyright notice in the Description page of Project Settings.


#include "HS/Weapons/Gun.h"

#include "Bullet.h"

AGun::AGun()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>GunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Meshes/Jericho_941.Jericho_941'"));
	if (GunMesh.Succeeded())
	{
		WeaponMesh->SetSkeletalMesh(GunMesh.Object);
	}
}

void AGun::Fire_Implementation()
{
	if (!BulletClass) return;

	// 총구 위치 가져오기
	FVector MuzzleLocation = WeaponMesh->GetSocketLocation(TEXT("MuzzleSocket"));
	FRotator MuzzleRotation = WeaponMesh->GetSocketRotation(TEXT("MuzzleSocket"));

	// 총알 생성
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; // 총알의 소유자를 총으로 설정
	SpawnParams.Instigator = GetInstigator(); // 플레이어가 총알을 발사했음을 설정

	ABullet* SpawnedBullet = GetWorld()->SpawnActor<ABullet>(BulletClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	
	if (SpawnedBullet)
	{
		SpawnedBullet->SetOwner(this);
	//	SpawnedBullet->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // 플레이어와 충돌 방지
	}
//	FVector MuzzleLocation = MeshComp->GetSocketLocation("head");
//	FRotator MuzzleRotation = MeshComp->GetSocketRotation("head");
//	//GetWorld()->SpawnActor<AActor>(/*총알클래스*/, MuzzleLocation, MuzzleRotation);
}

