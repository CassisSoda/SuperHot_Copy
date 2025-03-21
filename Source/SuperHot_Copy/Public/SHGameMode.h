// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SHGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_COPY_API ASHGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	ASHGameMode();

	virtual void Tick(float DeltaTime) override;
};
