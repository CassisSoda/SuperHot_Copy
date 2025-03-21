// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HS/Weapons/WeaponBase.h"
#include "Gun.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_COPY_API AGun : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AGun();

	virtual void Fire_Implementation() override;

protected:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun")
	//TSubclassOf<AActor> 총알;








};

