// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SHEnemy.generated.h"

UCLASS()
class SUPERHOT_COPY_API ASHEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASHEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void Fire();
	void RotateTowardsPlayer();

	UPROPERTY(EditAnywhere)
	float TimeBetweenShots = 1.0f;  // 블루프린트 변수 변환

	FTimerHandle FireTimerHandle;

	UPROPERTY()
	APawn* PlayerRef;

	UPROPERTY(EditAnywhere)
	bool bIsDead = false;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class ABullet> BulletClass; // 블루프린트에서 설정할 수 있도록

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* FirePoint; // 발사 위치 지정

	void AttachWeapon();

	UPROPERTY(EditAnywhere, Category = "Components")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AActor> GunClass;  // BP_Gun을 설정할 변수

	UPROPERTY()
	AActor* EquippedWeapon;
};
