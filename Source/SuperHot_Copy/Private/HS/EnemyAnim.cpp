// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperHot_Copy/Public/HS/EnemyAnim.h"

#include "HS/Enemy.h"


void UEnemyAnim::AnimNotify_AttackEnd()
{
	if (AEnemy* Enemy = Cast<AEnemy>(TryGetPawnOwner()))
	{
		Enemy->FSM->OnAttackEnd();
	}
}
