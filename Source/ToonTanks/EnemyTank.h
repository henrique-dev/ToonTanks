// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "EnemyTank.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AEnemyTank : public ABasePawn
{
	GENERATED_BODY()
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

	void SetEnabled(bool bEnable);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	class ATank* Tank;

	UPROPERTY(EditAnywhere, Category = "IA")
		float FireRange = 0.f;

	UPROPERTY(EditAnywhere, Category = "IA")
		float ChasingRange = 0.f;

	UPROPERTY(EditAnywhere, Category = "IA")
		float FollowSpeed = 0.f;

	UPROPERTY(EditAnywhere, Category = "IA")
		float AutoMoveSpeed = 0.f;

	FTimerHandle FireRateTimeHandle;
	float FireRate = 2.f;

	void CheckFireCondition();
	void FollowTarget();
	void Rotate(FVector LocationToRotate, float Speed);

	bool InFireRange();
	bool InChasingRange();

	bool bIsEnabled = false;
	bool bIsAutoMoving = false;
	FVector AutoMovingDirection;
};
