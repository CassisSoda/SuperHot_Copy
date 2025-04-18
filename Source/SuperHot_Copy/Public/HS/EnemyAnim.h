// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyFSM.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_COPY_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Fsm)
	EEnemyState AnimState = EEnemyState::Idle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category =FSM)
	class UAnimMontage* EnemyMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	bool bAttackPlay;

	UFUNCTION(	)
	void AnimNotify_AttackEnd();
	


	
};
