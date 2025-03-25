// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class SUPERHOT_COPY_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void Fire();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void Equip(AActor* Character);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void Unequip();

	
protected:
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Weapon")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	float Range;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	UStaticMeshComponent* BatMesh;



};
