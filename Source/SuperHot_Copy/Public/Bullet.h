// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class SUPERHOT_COPY_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	FVector StartPos;
	UPROPERTY(EditAnywhere)
	FVector EndPos;

	FVector Destination;

	UPROPERTY(EditAnywhere)
	float Speed = 500.f;

};
