// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SPowerUpActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASPowerUpActor : public AActor,
                                           public ISGameplayInterface {
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  ASPowerUpActor();

protected:
  UPROPERTY(VisibleAnywhere, Category = "Component")
  UStaticMeshComponent *MeshComp;

  FTimerHandle TimerHandle_CoolDown;

  UPROPERTY(EditDefaultsOnly, Category = "CoolDown");
  float CoolDownTime;

  UFUNCTION()
  void ShowPowerUp();

  void HideAndCoolDownPowerUp();

  void SetPowerUpState(bool bShouldActive);

public:
  void Interact_Implementation(APawn *InstigatorPawn) override;
};
