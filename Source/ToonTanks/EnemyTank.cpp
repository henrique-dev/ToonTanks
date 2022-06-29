// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTank.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Called when the game starts or when spawned
void AEnemyTank::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(FireRateTimeHandle, this, &AEnemyTank::CheckFireCondition, FireRate, true);

	UE_LOG(LogTemp, Warning, TEXT("ENEMY TANK OK"));
}

// Called every frame
void AEnemyTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsEnabled) return;

	if (InFireRange())
	{
		RotateTurret(Tank->GetActorLocation());
		bIsAutoMoving = false;
	}
	if (InChasingRange())
	{
		FollowTarget();
		bIsAutoMoving = false;
	}
	else
	{		

		if (!bIsAutoMoving)
		{
			if (Tank)
			{
				bIsAutoMoving = true;
				int32 X = FMath::RandRange(-2200, 2200);
				int32 Y = FMath::RandRange(-2200, 2200);
				AutoMovingDirection = FVector(Y, X, GetActorLocation().Z);
			}			
		}
		else
		{			
			Rotate(AutoMovingDirection, AutoMoveSpeed);
			if (GetActorLocation().Equals(AutoMovingDirection, 20))
			{
				bIsAutoMoving = false;
			}
		}

		int32 Change = FMath::RandRange(0, 500);
		if (Change < 5) bIsAutoMoving = false;
	}
}

void AEnemyTank::HandleDestruction()
{	
	Super::HandleDestruction();
	Destroy();
}

void AEnemyTank::SetEnabled(bool bEnable)
{	
	bIsEnabled = bEnable;
}

void AEnemyTank::CheckFireCondition()
{
	if (Tank == nullptr)
	{
		return;
	}

	if (InFireRange() && Tank->bAlive)
	{
		Fire();
	}
}

void AEnemyTank::FollowTarget()
{	
	if (Tank)
	{
		Rotate(Tank->GetActorLocation(), FollowSpeed);
	}	
}

void AEnemyTank::Rotate(FVector LocationToRotate, float Speed)
{
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	FVector ToTarget = LocationToRotate - BaseMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);

	BaseMesh->SetWorldRotation(
		FMath::RInterpTo(
			BaseMesh->GetComponentRotation(),
			LookAtRotation,
			DeltaTime,
			2.f
		)
	);
	SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), LocationToRotate, DeltaTime, Speed));
}

bool AEnemyTank::InFireRange()
{
	if (Tank)
	{
		return FVector::Dist(GetActorLocation(), Tank->GetActorLocation()) < FireRange;
	}
	return false;
}

bool AEnemyTank::InChasingRange()
{
	if (Tank && Tank->bAlive)
	{
		return FVector::Dist(GetActorLocation(), Tank->GetActorLocation()) < ChasingRange;
	}
	return false;
}
