// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperHot_Copy/Public/HS/EnemyFSM.h"

#include "Animation/AnimInstance.h"
#include "HS/Weapons/WeaponBase.h"
#include "SuperHot_Copy/Public/HS/Enemy.h"
#include "SuperHot_Copy/Public/HS/EnemyAnim.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<AEnemy>(GetOwner());
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, logMsg);

	switch (mState)
	{
		case EEnemyState::Idle:		{ IdleState();	 }	break;
		case EEnemyState::Chase:	{ ChaseState();	 }	break;
		case EEnemyState::Attack:	{ AttackState(); }	break;
		case EEnemyState::Damage:	{ DamageState(); }	break;
		case EEnemyState::Die:		{ DieState();	 }	break;
	}
}

void UEnemyFSM::IdleState()
{
	//me->PlayAnimMontage(anim->EnemyMontage, 1.0f, TEXT("Idle"));
}

void UEnemyFSM::ChaseState()
{
}

void UEnemyFSM::AttackState()
{
	if (Weapon)
	{
		Weapon->Fire();
	}
}

void UEnemyFSM::DamageState()
{
}

void UEnemyFSM::DieState()
{
}

