// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperHot_Copy/Public/HS/EnemyFSM.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "SuperHot_Copy/Public/HS/Enemy.h"
#include "SuperHot_Copy/Public/HS/EnemyAnim.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ASHPlayer::StaticClass());
	target = Cast<ASHPlayer>(actor);
	
	me = Cast<AEnemy>(GetOwner());
	if (me)
	{
		anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
		AController* Controller = me->GetController();
		if (Controller)
		{
			ai = Cast<AAIController>(Controller);
		}
	}
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, logMsg);

	switch (mState)
	{
		case EEnemyState::Idle:		{ IdleState();	 }	break;
		case EEnemyState::Chase:	{ ChaseState();	 }	break;
		case EEnemyState::Attack:	{ AttackState(); }	break;
		case EEnemyState::Damage:	{ DamageState(); }	break;
		case EEnemyState::Die:		{ DieState();	 }	break;
	}
}

void UEnemyFSM::IdleState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > IdleDelayTime)
	{
		mState = EEnemyState::Chase;
		currentTime = 0.0f;
		anim->AnimState = mState;
	}
}

void UEnemyFSM::ChaseState()
{
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();

	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FPathFindingQuery query;
	FAIMoveRequest req;
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);
	ai->BuildPathfindingQuery( req, query );
	FPathFindingResult r = ns->FindPathSync(query);
	
	if (r.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation(destination);
	}
	else
	{
		auto result = ai->MoveToLocation(randomPos);
		
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500.0f, randomPos);
		}
	}
	if ( dir.Size() < AttackRange )
	{
		// 길 찾기 기능 정지
		ai->StopMovement();
		// 공격 상태로 전환하고 싶다.
		mState = EEnemyState::Attack;
		// 애니메이션 상태 동기화
		anim->AnimState = mState;
		
		// 공격 애니메이션 재생 활성화
		anim->bAttackPlay = true;
		// 공격 상태 전환시 대기 시간 없이 바로 플레이가 되도록
		currentTime = attackDelayTime;
	}
}


void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	
	if ( currentTime > attackDelayTime )
	{
		currentTime = 0.0f;
		anim->bAttackPlay = true;
		// 공격 시작 시 콜리전 활성화
		me->EnableAttackCollision();
	}
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	if ( distance > AttackRange )
	{
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500.0f, randomPos);
		mState = EEnemyState::Chase;
		anim->AnimState = mState;
	}	
}

void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if ( currentTime > damageDelayTime )
	{
		mState = EEnemyState::Idle;
		currentTime = 0.0f;
		anim->AnimState = mState;
	}
}

void UEnemyFSM::DieState()
{
	// 이미 죽었다면 실행 안 함
	if (bIsDead) return;
	bIsDead = true;  // 사망 상태 플래그 설정

	//// SkeletalMesh 숨기기
	//GetMesh()->SetVisibility(false);
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//
	//if (GeometryCollectionComp)
	//{
	//	GeometryCollectionComp->SetVisibility(true);
	//	GeometryCollectionComp->SetSimulatePhysics(true);
//
	//	GeometryCollectionComp->SetEnableGravity(true);
	//	GeometryCollectionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("GeometryCollectionComp is NULL!"));
	//}
	// 일정 시간 후 파편 제거
	//GetWorld()->GetTimerManager().SetTimer(DestructionTimerHandle, this, &ASHEnemy::DestroyFragments, 5.0f, false);
}

void UEnemyFSM::onDamageProcess()		// 플레이어에서 호출
{
	--hp;
	
	if (hp > 0)
	{
		mState = EEnemyState::Damage;
	}
	else
	{
		mState = EEnemyState::Die;
	}
	ai->StopMovement();
	anim->AnimState = mState;
}

void UEnemyFSM::OnAttackEnd()
{
	anim->bAttackPlay = false;
	// 공격 애니메이션 종료 후 콜리전 비활성화
	me->DisableAttackCollision();
}

bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);

	dest = loc.Location;
	return result; 
}



