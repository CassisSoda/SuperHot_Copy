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
	
	float playerHP = 1.f;

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

	bool isGrabbing = false;
	bool isDead = false;

	FVector PrePos;
	FQuat PreRot;

	UPROPERTY(EditAnywhere, Category = "Grab")
	float ThrowPower = 500.f;
	
	UPROPERTY(EditAnywhere, Category = "Grab")
	float ToquePower = 500.f;

	class AEnemy* enemy;


#pragma endregion Values

	UPROPERTY(EditAnywhere, Category = "Grab")
	float GrabRadius = 100.f;

	UPROPERTY()
	class UPrimitiveComponent* GrabObjectComp;

	UPROPERTY()
	class AActor* GrabObject;

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
	class UMotionControllerComponent* RightHandAim;

	UPROPERTY(VisibleAnywhere, Category = "Default")
	class USkeletalMeshComponent* RightHandMesh;

	UPROPERTY(VisibleAnywhere, Category = "Default")
	class USphereComponent* RightHandCollision;

	UPROPERTY()
	class UHandAnimInstance* LeftAnim;

	UPROPERTY()
	class UHandAnimInstance* RightAnim;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UChildActorComponent* CrosshairComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ASHGameMode* shGameMode;

#pragma endregion Default_Settings


#pragma region InputAction

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_PlayerMove;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_PlayerTurn;

	#pragma region Right_Btn

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_BPunch;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_RTrigger;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_RGrip;
	#pragma endregion Right_Btn

	#pragma region Left_Btn

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_YPunch;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LTrigger;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LGrip;

	#pragma endregion Left_Btn

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_RespawnPlayer;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_TestKill;

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

	void GunFire(const struct FInputActionValue& InputValue);

	void TestKill();



#pragma endregion InputActionFucntion

	void ShiftDilation();
	UFUNCTION()
	void OnEnemyOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void TryGrab();
	void GrabActor(AActor* actor);
	void Grabbing();
	void TryRelease();

	void DrawCrosshair();

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void ClearPlayerState();

	//FORCEINLINE FRotator 

	FVector	ThrowDirection;
	FQuat DeltaRotation;


#pragma region 스테이지
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void Respawn();

	UPROPERTY(EditAnywhere, Category = "Respawn")
	FVector RespawnLocation;

#pragma endregion 


#pragma region 데미지테스트
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_TESTDamage;

	void ApplyDamageToEnemy();
#pragma endregion 




};
