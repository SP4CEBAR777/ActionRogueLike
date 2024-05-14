// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.h"
#include "SPowerUp_HealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASPowerUp_HealthPotion : public ASPowerUpActor {
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  ASPowerUp_HealthPotion();

protected:
  UPROPERTY(EditDefaultsOnly, Category = "HealthPotion")
  float HealAmount;

  UPROPERTY(EditDefaultsOnly, Category = "HealthPotion")
  int32 CreditCost;

public:
  void Interact_Implementation(APawn *InstigatorPawn) override;

  FText GetInteractText_Implementation(APawn *InstigatorPawn) override;
};
