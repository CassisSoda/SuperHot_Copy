// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialWidget.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_COPY_API UTutorialWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UTutorialWidget(const FObjectInitializer& ObjectInitializer);

	// Tick 이벤트 오버라이드
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual void NativeConstruct() override; // 위젯 초기화 시 호출

public:
	
	bool isMoved = false;
	bool isGripedAndTriggered = false;
	bool isFiredDone = false;

	UPROPERTY( EditDefaultsOnly, Category = "TextBox")
	class UTextBlock* Move_Text;

	UPROPERTY(EditDefaultsOnly, Category = "TextBox")
	class UTextBlock* Grip_Text;

	UPROPERTY( EditDefaultsOnly, Category = "TextBox")
	class UTextBlock* Fire_Text;

	UPROPERTY()
	TArray<UTextBlock*> TextArray;

	UPROPERTY()
	class ASHPlayer* player;

};
