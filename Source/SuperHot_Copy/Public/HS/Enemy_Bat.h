// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HS/Enemy.h"
#include "Enemy_Bat.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_COPY_API AEnemy_Bat : public AEnemy
{
	GENERATED_BODY()


public:
	AEnemy_Bat();

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, Category="Components")
	
	UStaticMeshComponent* BatMesh;
	
};
