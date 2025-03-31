// Fill out your copyright notice in the Description page of Project Settings.


#include "SHGameMode.h"
#include "SHPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "shDebug.h"

void ASHGameMode::BeginPlay()
{
	
}

ASHGameMode::ASHGameMode()
{
	CurrentStage = 1;
	CurrentStageName = "1";
	EnemyCount = 0;

	RespawnLocation.Add(1, FVector(-1100, 855, 65.000100));
	RespawnLocation.Add(2, FVector(790, -445, 581.271706));
	RespawnLocation.Add(3, FVector(2015,-80,273.473420));

	StageEnemyCounts.Add(1, 2);	// 스테이지 1 적 개수
	StageEnemyCounts.Add(2, 4);	// 스테이지 2 적 개수
	StageEnemyCounts.Add(3, 6);	// 스테이지 3 적 개수

}

void ASHGameMode::Tick(float DeltaTime)
{

}


void ASHGameMode::StageReLoad()
{
	player = Cast<ASHPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld (), 0));
	if (player)
	{
		Debug::Print ("RESPAWN Complete!!!");
		player->SetActorLocation (GetRespawnLocation ());
		
	}
}
void ASHGameMode::CheckStageClear()
{
	if (EnemyCount <=0)
	{
		LoadNextStage();
	}
}

void ASHGameMode::OnEnemyKilled()
{
	EnemyCount--;
	CheckStageClear();
}

FVector ASHGameMode::GetRespawnLocation()
{
	if (RespawnLocation.Contains(CurrentStage))
	{
		return RespawnLocation[CurrentStage];
	}
	return FVector::ZeroVector;
}

void ASHGameMode::FadeInOut()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
		if (CameraManager)
		{
			CameraManager->StartCameraFade(1, 0, 1.0f, FLinearColor::Black, false, true);
		}
	}
}

void ASHGameMode::LoadNextStage()
{
	// 현재 스테이지 제거
	UGameplayStatics::UnloadStreamLevel(this, CurrentStageName, FLatentActionInfo(), true);

	// 다음 스테이지 이동
	if (CurrentStage == 1)
	{
		CurrentStageName = "2";
		StageReLoad ();
	}
	else if (CurrentStage == 2)
	{
		CurrentStageName = "3";
		StageReLoad ();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CLEAR"));
		return;
	}

	// 다음 스테이지 로드
	UGameplayStatics::LoadStreamLevel(this,CurrentStageName,true,true,FLatentActionInfo());
	CurrentStage++;
	
	// 새로운 적 개수 설정
	if (StageEnemyCounts.Contains(CurrentStage))
	{
		EnemyCount = StageEnemyCounts[CurrentStage]; // 스테이지별 적 개수 적용
	}
	else
	{
		EnemyCount = 5; // 기본값 (혹시 맵에 값이 없을 경우 대비)
	}

}
