// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "SHPlayer.h"

UTutorialWidget::UTutorialWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
	Move_Text = nullptr;
	Grip_Text = nullptr;
	Fire_Text = nullptr;


}

void UTutorialWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Move_Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Move")));
	Grip_Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Grip")));
	Fire_Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Fire")));
	player = Cast<ASHPlayer> (UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (Move_Text)
	{
		Move_Text->SetVisibility (ESlateVisibility::Visible);
		TextArray.Add(Move_Text);

	}

	if (Grip_Text)
	{
		Grip_Text->SetVisibility (ESlateVisibility :: Hidden);
		TextArray.Add(Grip_Text);
	}

	if (Fire_Text)
	{
		Fire_Text->SetVisibility(ESlateVisibility::Hidden);
		TextArray.Add(Fire_Text);
	}

}


void UTutorialWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//if (player)
	//{
	//	if (isMoved && isGripedAndTriggered && isFiredDone) return;

	//	if (!isMoved)
	//	{
	//		if (player->isMoving)
	//		{
	//			isMoved = true;
	//			Move_Text->SetVisibility (ESlateVisibility::Hidden);
	//			Grip_Text->SetVisibility (ESlateVisibility::Visible);
	//		}
	//	}

	//	if (isMoved && !isGripedAndTriggered)
	//	{
	//		if (player->isGrabbing)
	//		{
	//			//isGripedAndTriggered = true;
	//			Grip_Text->SetVisibility (ESlateVisibility::Hidden);
	//			Fire_Text->SetVisibility (ESlateVisibility::Visible);
	//		}
	//		else
	//		{
	//			Grip_Text->SetVisibility(ESlateVisibility::Visible);
	//			Fire_Text->SetVisibility(ESlateVisibility::Hidden);
	//		}
	//	}
	//	if (!isFiredDone)
	//	{
	//		if (player->isGrabbing && player->isRTriggerPressed)
	//		{
	//			isGripedAndTriggered = true;
	//			isFiredDone = true;

	//			Grip_Text->SetVisibility(ESlateVisibility::Hidden);
	//			Fire_Text->SetVisibility(ESlateVisibility::Hidden);
	//		}
	//	}

	//	
	//}
}	
