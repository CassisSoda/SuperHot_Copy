// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	class UProjectileMovementComponent* Movement;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	float LifeTime = 5.f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	float Speed = 1500.f;

	class ASHPlayer* player;
	class AEnemy* enemy;

	void SetVelocity(FVector value);

	UFUNCTION()
    void OnBulletOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
	

};
