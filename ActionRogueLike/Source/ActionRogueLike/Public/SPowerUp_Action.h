// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUpActor.h"
#include "SPowerUp_Action.generated.h"

class USAction;

/**
 *
 */
UCLASS()
class ACTIONROGUELIKE_API ASPowerUp_Action : public ASPowerUpActor {
  GENERATED_BODY()

protected:
  UPROPERTY(EditDefaultsOnly, Category = "Action")
  TSubclassOf<USAction> ActionToGrant;

public:
  void Interact_Implementation(APawn *InstigatorPawn) override;
};
