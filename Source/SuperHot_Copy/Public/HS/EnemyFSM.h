// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SHPlayer.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle UMETA( DisplayName = "대기"),
	Chase UMETA( DisplayName = "이동"),
	Attack UMETA( DisplayName = "공격"),
	Damage UMETA( DisplayName = "데미지"),
	Die UMETA( DisplayName = "죽음")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUPERHOT_COPY_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	EEnemyState mState = EEnemyState::Idle;

	void IdleState();
	void ChaseState();
	void AttackState();
	void DamageState();
	void DieState();

	void onDamageProcess();

	void OnAttackEnd();
	
	UPROPERTY()
	class AEnemy* me;

	UPROPERTY()
	class UEnemyAnim* anim;
	
	//class AWeaponBase* Weapon;
	
	UPROPERTY(VisibleAnywhere,Category=FSM)
	class ASHPlayer* target;

	UPROPERTY(EditAnywhere, Category=FSM)
	float AttackRange = 500.0f;
	
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackDelayTime = 1.5f;

	UPROPERTY(EditAnywhere, Category=FSM)
	float currentTime = 0.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
	float DetectRange = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	int32 hp = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	float Speed = 250.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float IdleDelayTime = 2.0f;
	
	UPROPERTY()
	APawn* TargetPlayer = nullptr; // 감지된 플레이어

	UPROPERTY()
	class AAIController* ai;

	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 2.0f;
	
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

	FVector randomPos;


private:
	UPROPERTY(EditAnywhere)
	bool bIsDead = false;


};

