// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SHPlayer.h"
#include "shDebug.h"
#include "Kismet/GameplayStatics.h"
#include "HS/Enemy.h"
#include "HS/SHEnemy.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject <UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent (CapsuleComp);

	CapsuleComp->SetCapsuleHalfHeight (4.f);
	CapsuleComp->SetCapsuleRadius (2.f);

	MeshComp = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment (RootComponent);
	MeshComp->SetRelativeLocation (FVector(7.55f, 0.f, -7.25f));


	//ProjectileMovement 부착
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->ProjectileGravityScale = 0.0f;
	Movement->UpdatedComponent = RootComponent;

	ConstructorHelpers::FObjectFinder<UStaticMeshComponent> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/MW/Assets/Bullet/SM_Bullet.SM_Bullet'"));
	if (TempMesh.Succeeded())
	{
		MeshComp = TempMesh.Object;
	}
	//CapsuleComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);  // Bullet 채널
	//CapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Ignore);  // Gun 채널 무시
	CapsuleComp->SetCollisionProfileName (TEXT("Bullet"));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	player = Cast<ASHPlayer>(GetOwner());
	SetLifeSpan (LifeTime);

	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlap);

	Debug::Print("Fire!!!");
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::SetVelocity(FVector value)
{
	Movement->Velocity = value * Speed;
}

void ABullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//enemy = Cast<AEnemy>(OtherActor);

	if(OtherActor)
	{
		Debug::Print (OtherActor->GetName ());
		UE_LOG(LogTemp, Warning, TEXT("Location: %.2f, %.2f, %.2f"), SweepResult.Location.X, SweepResult.Location.Y, SweepResult.Location.Z);
		Debug::Print(player->GetInstigatorController()->GetName ());
		UGameplayStatics::ApplyPointDamage (OtherActor, 1.f, SweepResult.Location, SweepResult, player->GetInstigatorController(), player, UDamageType::StaticClass ());
		Debug::Print("Bullet Attack!!!!");
	}

	// 적이 맞았는지 확인
	ASHEnemy* SHEnemy = Cast<ASHEnemy>(OtherActor);
	if (SHEnemy)
	{
		SHEnemy->OnDeath(); // 적을 파편화
		Destroy(); // 총알 제거
	}
}

