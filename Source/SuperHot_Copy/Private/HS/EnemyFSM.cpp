// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperHot_Copy/Public/HS/EnemyFSM.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
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
//	if (!target || !me) return;
//
//	// 플레이어와의 거리 계산
//	float Distance = FVector::Dist(me->GetActorLocation(), target->GetActorLocation());
//
//	// 만약 플레이어가 감지 범위 안으로 들어오면
//	if (Distance < DetectRange)
//	{
//		mState = EEnemyState::Chase;
//		anim->AnimState = mState;
//		return;
//	}
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
//	if (!target || !ai || !me) return;
//
//	FVector PlayerLocation = target->GetActorLocation();
//	FVector EnemyLocation = me->GetActorLocation();
//	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
//
//	if (distance <= AttackRange)
//	{
//		mState = EEnemyState::Attack;
//		anim->AnimState = mState;
//		return;
//	}
//	
//	me->GetCharacterMovement()->MaxWalkSpeed = Speed;	// 속도 조절
//	
//	FVector Direction = (PlayerLocation - EnemyLocation).GetSafeNormal(); // 정규화된 방향 벡터
//
//	FVector Destination = EnemyLocation + Direction * 100.0f;
//	
//	FNavLocation NavLocation;
//	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
//	if (NavSystem && NavSystem->ProjectPointToNavigation(Destination, NavLocation))
//	{
//		ai->MoveToLocation(NavLocation.Location);
//	}
}


void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	
	if ( currentTime > attackDelayTime )
	{
		currentTime = 0.0f;
		anim->bAttackPlay = true;
	}
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	if ( distance > AttackRange )
	{
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500.0f, randomPos);
		mState = EEnemyState::Chase;
		anim->AnimState = mState;
	}	
//	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
//	anim->AnimState = EEnemyState::Attack;
//	
//	currentTime += GetWorld()->DeltaTimeSeconds;
//	if (currentTime >= attackDelayTime)
//	{
//		if (distance > AttackRange) // 플레이어가 멀어지면 추격 상태로 복귀
//		{
//			mState = EEnemyState::Chase;
//		}
//		else
//		{
//			mState = EEnemyState::Idle; // 공격 후 잠시 대기
//		}
//
//		anim->AnimState = mState;
//		//anim->bAttackPlay = false; // 공격 애니메이션 종료
//		currentTime = 0.0f; // 타이머 초기화
//	}
////	if (!target || !Weapon) return;
////
////	me->PlayAnimMontage(anim->EnemyMontage, 1.0f, TEXT("Attack"));
////	Weapon->Fire();
////
////	float Distance = FVector::Dist(me->GetActorLocation(), target->GetActorLocation());
////	if (currentTime >= attackDelayTime)
////	{
////		if (Distance > 300.0f)
////		{
////			mState = EEnemyState::Chase;
////		}
////	}
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
	me->Destroy();
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
}

bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);

	dest = loc.Location;
	return result; 
}



