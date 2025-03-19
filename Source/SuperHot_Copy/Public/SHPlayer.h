// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SHPlayer.generated.h"

UCLASS()
class SUPERHOT_COPY_API ASHPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASHPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = "Default")
	class UCameraComponent* VRCamera;



#pragma region InputAction

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_PlayerMove;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_PlayerTurn;

#pragma endregion InputAction

#pragma region InputActionFucntion

	void Move(const struct FInputActionValue& InputValue);
	void Turn(const struct FInputActionValue& InputValue);


#pragma endregion InputActionFucntion

};
