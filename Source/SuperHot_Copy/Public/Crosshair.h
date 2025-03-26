// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Crosshair.generated.h"


UENUM(BlueprintType)
enum class CrosshairType : uint8
{
	dotType		UMETA(DisplayName = "Dot"),
	crossType	UMETA(DisplayName = "Cross")
};


UCLASS()
class SUPERHOT_COPY_API ACrosshair : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrosshair();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Default")
	class USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, Category = "Default")
	class UStaticMeshComponent* DotPlane;

	UPROPERTY(EditAnywhere, Category = "Default")
	class UStaticMeshComponent* CrossPlane;

	CrosshairType Chtype = CrosshairType:: dotType;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TArray <UStaticMeshComponent*> childrenComp;

	void ChangeType();
	void DrawDot();
	void DrawCross();



};
