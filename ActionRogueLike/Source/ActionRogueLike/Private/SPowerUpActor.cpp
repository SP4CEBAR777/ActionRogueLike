// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerUpActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASPowerUpActor::ASPowerUpActor() {
  MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
  RootComponent = MeshComp;

  CoolDownTime = 10.0f;
}

void ASPowerUpActor::Interact_Implementation(APawn *InstigatorPawn) {}

void ASPowerUpActor::ShowPowerUp() { SetPowerUpState(true); }

void ASPowerUpActor::HideAndCoolDownPowerUp() {
  SetPowerUpState(false);
  GetWorldTimerManager().SetTimer(TimerHandle_CoolDown, this,
                                  &ASPowerUpActor::ShowPowerUp, CoolDownTime);
}

void ASPowerUpActor::SetPowerUpState(bool bShouldActive) {
  RootComponent->SetVisibility(bShouldActive, true);
}
