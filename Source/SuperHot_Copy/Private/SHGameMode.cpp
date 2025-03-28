// Fill out your copyright notice in the Description page of Project Settings.


#include "SHGameMode.h"
#include "SHPlayer.h"
#include "../shDebug.h"
#include "Kismet/GameplayStatics.h"

ASHGameMode::ASHGameMode()
{

}
void ASHGameMode::BeginPlay()
{
	Super::BeginPlay();


	FVector Stage1Point = FVector(0.f,0.f,0.f);
	FVector Stage2Point = FVector();
	FVector Stage3Point = FVector();

	PlayerRespawnPoints.Add(Stage1Point);
	PlayerRespawnPoints.Add(Stage2Point);
	PlayerRespawnPoints.Add(Stage3Point);

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
		player->SetActorLocation (PlayerRespawnPoints[StageClear]);
		
	}
	
}
