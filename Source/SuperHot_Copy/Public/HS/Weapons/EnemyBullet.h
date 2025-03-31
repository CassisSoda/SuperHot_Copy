// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Actor.h"
#include "EnemyBullet.generated.h"

UCLASS()
class SUPERHOT_COPY_API AEnemyBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	FVector EndPos;        // 목표 위치
	FVector Destination;   // 이동 방향
	float Speed = 450.0f; // 속도
	
public:
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* BulletTrailEffect;
	
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
};
