// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "EnemyTank.h"
#include "ToonTanksPlayerController.h"

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		if (ToonTanksPlayerController)
		{
			ToonTanksPlayerController->SetPlayerEnabledState(false);
		}
		GameOver(false);
	}
	else if (ATower* DestroyedEnemy = Cast<ATower>(DeadActor))
	{
		DestroyedEnemy->HandleDestruction();
		TargetEnemies--;

		if (TargetEnemies == 0)
		{
			GameOver(true);
		}
	}
	else if (AEnemyTank* EnemyTank = Cast<AEnemyTank>(DeadActor))
	{
		EnemyTank->HandleDestruction();
		TargetEnemies--;

		if (TargetEnemies == 0)
		{
			GameOver(true);
		}
	}
}

void AToonTanksGameMode::HandleGameStart()
{
	TargetEnemies = GetTargetEnemiesCount();
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();

	if (ToonTanksPlayerController)
	{
		ToonTanksPlayerController->SetPlayerEnabledState(false);

		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate  PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
			ToonTanksPlayerController,
			&AToonTanksPlayerController::SetPlayerEnabledState,
			true
		);
		GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, PlayerEnableTimerDelegate, StartDelay, false);
		GetWorldTimerManager().SetTimer(EnableEnemiesTimeHandle, this, &AToonTanksGameMode::EnableEnemies, StartDelay, false);
	}
}

void AToonTanksGameMode::EnableEnemies()
{
	TArray<AActor*> EnemyTanks;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemyTank::StaticClass(), EnemyTanks);

	for (AActor* Actor : EnemyTanks)
	{
		AEnemyTank* EnemyTank = Cast<AEnemyTank>(Actor);

		EnemyTank->SetEnabled(true);
	}
}

int32 AToonTanksGameMode::GetTargetEnemiesCount()
{
	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);

	TArray<AActor*> EnemyTanks;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemyTank::StaticClass(), EnemyTanks);

	return Towers.Num() + EnemyTanks.Num();
}