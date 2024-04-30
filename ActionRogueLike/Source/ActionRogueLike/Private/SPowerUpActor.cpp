// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerUpActor.h"
#include "Components/SphereComponent.h"

// Sets default values
ASPowerUpActor::ASPowerUpActor() {
  SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
  RootComponent = SphereComp;
  MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
  MeshComp->SetupAttachment(RootComponent);

  MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

  CoolDownTime = 10.0f;
}

void ASPowerUpActor::Interact_Implementation(APawn *InstigatorPawn) {}

void ASPowerUpActor::ShowPowerUp() { SetPowerUpState(true); }

void ASPowerUpActor::HideAndCoolDownPowerUp() {
  SetPowerUpState(false);

  GetWorldTimerManager().SetTimer(TimerHandle_CoolDownTimer, this,
                                  &ASPowerUpActor::ShowPowerUp, CoolDownTime);
}

void ASPowerUpActor::SetPowerUpState(bool bShouldActive) {
  SetActorEnableCollision(bShouldActive);

  RootComponent->SetVisibility(bShouldActive, true);
}
