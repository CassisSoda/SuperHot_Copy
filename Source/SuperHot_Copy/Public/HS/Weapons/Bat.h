// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HS/Weapons/WeaponBase.h"
#include "Bat.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_COPY_API ABat : public AWeaponBase
{
	GENERATED_BODY()

public:
	ABat();

	virtual void Fire_Implementation() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bat")
	float SwingDuration;
	
};
