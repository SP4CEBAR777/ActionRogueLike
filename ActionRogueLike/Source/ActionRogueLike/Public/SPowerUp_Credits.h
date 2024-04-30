// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUpActor.h"
#include "SPowerUp_Credits.generated.h"

/**
 *
 */
UCLASS()
class ACTIONROGUELIKE_API ASPowerUp_Credits : public ASPowerUpActor {
  GENERATED_BODY()

protected:
  UPROPERTY(EditAnywhere, Category = "Coin")
  int32 CreditsAmount;

public:
  ASPowerUp_Credits();

  void Interact_Implementation(APawn *InstigatorPawn) override;
};
