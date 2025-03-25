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


#pragma region Values

	bool isDelay = true;

	bool isBPressed = false;
	bool isRTriggerPressed = false;
	bool isRGripPressed = false;
	bool bRightPunch = false;
	TArray<FKey> RightPressedKeys;

	bool isYPressed = false;
	bool isLTriggerPressed = false;
	bool isLGripPressed = false;
	bool bLeftPunch = false;
	TArray<FKey> LeftPressedKeys;

	bool isGrapping = false;


#pragma endregion Values

#pragma region Default_Settings

	UPROPERTY(VisibleAnywhere, Category = "Default")
	class UCameraComponent* VRCamera;

	UPROPERTY(VisibleAnywhere, Category = "Default")
	class UMotionControllerComponent* LeftHand;

	UPROPERTY(VisibleAnywhere, Category = "Default")
	class USkeletalMeshComponent* LeftHandMesh;

	UPROPERTY(VisibleAnywhere, Category = "Default")
	class USphereComponent* LeftHandCollision;

	UPROPERTY(VisibleAnywhere, Category = "Default")
	class UMotionControllerComponent* RightHand;

	UPROPERTY(VisibleAnywhere, Category = "Default")
	class USkeletalMeshComponent* RightHandMesh;

	UPROPERTY(VisibleAnywhere, Category = "Default")
	class USphereComponent* RightHandCollision;

	UPROPERTY()
	class UHandAnimInstance* LeftAnim;

	UPROPERTY()
	class UHandAnimInstance* RightAnim;

#pragma endregion Default_Settings


#pragma region InputAction

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_PlayerMove;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_PlayerTurn;

	#pragma region Right_Punch

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_BPunch;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_RTrigger;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_RGrip;
	#pragma endregion Right_Punch

	#pragma region Left_Punch

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_YPunch;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LTrigger;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LGrip;

	#pragma endregion Left_Punch
	

#pragma endregion InputAction

#pragma region InputActionFucntion

	void Move(const struct FInputActionValue& InputValue);
	void Turn(const struct FInputActionValue& InputValue);

	void OnBPressed(const struct FInputActionValue& InputValue);
	void OnBReleased(const struct FInputActionValue& InputValue);
	void OnRTriggerPressed(const struct FInputActionValue& InputValue);	
	void OnRTriggerReleased(const struct FInputActionValue& InputValue);
	void OnRGripPressed(const struct FInputActionValue& InputValue);
	void OnRGripReleased(const struct FInputActionValue& InputValue);

	void RightPunch();
	void ResetRightCombo();
	
	void OnYPressed(const struct FInputActionValue& InputValue);
	void OnYReleased(const struct FInputActionValue& InputValue);
	void OnLTriggerPressed(const struct FInputActionValue& InputValue);
	void OnLTriggerReleased(const struct FInputActionValue& InputValue);
	void OnLGripPressed(const struct FInputActionValue& InputValue);
	void OnLGripReleased(const struct FInputActionValue& InputValue);

	void LeftPunch();
	void ResetLeftCombo();


#pragma endregion InputActionFucntion

	void ShiftDilation();
	void OnEnemyOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DrawGrapStraight();
	void GrapActor(AActor* actor);





};
