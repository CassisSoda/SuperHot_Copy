// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBullet.h"
#include "GameFramework/Actor.h"
#include "EnemyGun.generated.h"

UCLASS()
class SUPERHOT_COPY_API AEnemyGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 발사 기능
	UFUNCTION()
	void Fire();
	


private:
	// 총구 위치를 위한 Scene Component
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

	// 총구 위치
	UPROPERTY(EditAnywhere, Category = "Gun")
	FVector MuzzleOffset;

	// 발사 가능 여부
	bool bCanFire = true;

	// 발사 간격
	UPROPERTY(EditAnywhere, Category = "Gun")
	float TimeBetweenShots = 0.5f;

	// 쿨다운 리셋 함수
	void ResetFireCooldown();

	// 플레이어가 들고 있는지?
	bool isPlayerGrabbing = false;
	
public:
	// 총구 이펙트
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* MuzzleEffect;

	// 애니메이션 재생
	UPROPERTY(EditAnywhere, Category = "Effects")
	UAnimMontage* FireAnimation;

	// 총 애니메이션을 재생할 메쉬
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* WeaponMesh;

	FTimerHandle FireCooldownTimer;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BulletMesh;

	// 총구 위치 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* FirePoint;
	
	// 발사체 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	TSubclassOf<AEnemyBullet> BulletClass;
	
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

	

private:
	APawn* PlayerRef;

	
};
