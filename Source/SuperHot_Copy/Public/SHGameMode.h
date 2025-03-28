// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SHGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_COPY_API ASHGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	ASHGameMode();

	virtual void Tick(float DeltaTime) override;

	int32 StageClear = 1;

	void StageReLoad();

	// 현재 스테이지 인덱스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stage")
	int32 CurrentStage;

	// 현재 스테이지 이름
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="Stage")
	FName CurrentStageName;

	// 적 개수 확인
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Stage")
	int32 EnemyCount;

	// 다음 스테이지 로드
	UFUNCTION(BlueprintCallable, Category="Stage")
	void LoadNextStage();
	// 적 죽었는지 확인
	void CheckStageClear();
	// 적 한마리씩 죽을 때마다 호출
	void OnEnemyKilled();

	// 리스폰 위치 저장
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Respawn")
	TMap<int32, FVector> RespawnLocation;

	FVector GetRespawnLocation();

	// 페이드 효과
	UFUNCTION(BlueprintCallable, Category="Stage")
	void FadeInOut();

private:
	UPROPERTY(EditAnywhere, Category="Stage")
	TMap<int32, int32> StageEnemyCounts;
	// 스테이지 별 적 개수 저장

	class ASHPlayer* player;
	
	
	
};
