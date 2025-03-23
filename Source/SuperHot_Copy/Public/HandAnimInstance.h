// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HandAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_COPY_API UHandAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float GripAlpha = 0.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float TriggerAlpha = 0.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool isPunching = false;
};
