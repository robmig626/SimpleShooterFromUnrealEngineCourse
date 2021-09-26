// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooterGameModeBase.h"
#include "KillEmAllGamemode.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AKillEmAllGamemode : public ASimpleShooterGameModeBase
{
	GENERATED_BODY()

public:

	virtual void PawnKilled(APawn* KilledPawn) override;
	
private:

	void EndGame(bool bIsPlayerWinner);
	
};
