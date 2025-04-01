// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "GeometryCollection/GeometryCollection.h"
#include "GameFramework/Character.h"
#include "Weapons/EnemyGun.h"
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
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	

private:
	void Fire();
	void RotateTowardsPlayer();
	void AttachWeapon();

	UPROPERTY(EditAnywhere)
	float TimeBetweenShots = 1.0f;  // 블루프린트 변수 변환

	FTimerHandle FireTimerHandle;
	FTimerHandle DestructionTimerHandle;  // 파편 제거용 타이머 핸들

	UPROPERTY()
	APawn* PlayerRef;

	UPROPERTY(EditAnywhere)
	bool bIsDead = false;

public:
	UPROPERTY(EditAnywhere, Category = "Components")
	USkeletalMeshComponent* SkeletalMesh;
	
	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* FirePoint; // 발사 위치 지정
	
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AActor> GunClass;  // BP_Gun을 설정할 변수
	
	// 총기 객체를 저장할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AEnemyGun* Gun;  // Gun을 저장할 변수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Geometry")
	UGeometryCollectionComponent* GeometryCollectionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health = 1.0f;

	UFUNCTION()
	void OnDeath();
	

	UFUNCTION()
	void DestroyFragments();


};
