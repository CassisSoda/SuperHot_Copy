// Fill out your copyright notice in the Description page of Project Settings.


#include "HS/Weapons/WeaponBase.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	BatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BatMesh"));
	RootComponent = BatMesh;
}



// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::Fire_Implementation()
{
}

void AWeaponBase::Equip_Implementation(AActor* Character)
{
//	AttachToComponent(Character->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}


void AWeaponBase::Unequip_Implementation()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}


