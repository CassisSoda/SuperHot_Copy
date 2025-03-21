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



protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	AWeaponBase* CurrentWeapon;
	
	
};
