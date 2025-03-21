// Fill out your copyright notice in the Description page of Project Settings.


#include "HS/Weapons/Gun.h"

AGun::AGun()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>GunMesh(TEXT("/Script/Engine.StaticMesh'/Game/HS/assets/gun/gun.gun'"));
	if (GunMesh.Succeeded())
	{
		WeaponMesh->SetStaticMesh(GunMesh.Object);
		WeaponMesh->SetWorldScale3D(FVector(0.25f));
	}
	
}

void AGun::Fire_Implementation()
{
	FVector MuzzleLocation = MeshComp->GetSocketLocation("head");
	FRotator MuzzleRotation = MeshComp->GetSocketRotation("head");
	//GetWorld()->SpawnActor<AActor>(/*총알클래스*/, MuzzleLocation, MuzzleRotation);
}

