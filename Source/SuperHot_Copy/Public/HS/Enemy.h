// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapons/WeaponBase.h"
#include "Enemy.generated.h"

UCLASS()
class SUPERHOT_COPY_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<AWeaponBase> DefaultWeaponClass;			// 기본 무기 클래스

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	AWeaponBase* EquippedWeapon;							// 현재 장착 무기

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeaponBase> WeaponClass;

	class AWeaponBase* Weapon;

	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSMComponent)
	class UEnemyFSM* FSM;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipWeapon();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void FireWeapon();

	UFUNCTION(BlueprintCallable, Category="Stage")
	void Die();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	AWeaponBase* CurrentWeapon;
	
public:
	// 공격용 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	class USphereComponent* AttackCollision;

	// 공격 히트 판정 함수
	UFUNCTION()
	void OnAttackHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult);

	// 공격 활성화 및 비활성화 함수
	void EnableAttackCollision();
	void DisableAttackCollision();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Geometry")
	UGeometryCollectionComponent* GeometryCollectionComp;
};
