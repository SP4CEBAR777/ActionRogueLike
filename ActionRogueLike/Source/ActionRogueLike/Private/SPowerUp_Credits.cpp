// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerUp_Credits.h"
#include "SPlayerState.h"
#include "SPowerupActor.h"

ASPowerUp_Credits::ASPowerUp_Credits() { CreditsAmount = 80; }

void ASPowerUp_Credits::Interact_Implementation(APawn *InstigatorPawn) {
  if (!ensure(InstigatorPawn)) {
    return;
  }

  if (ASPlayerState *PS = InstigatorPawn->GetPlayerState<ASPlayerState>()) {
    PS->AddCredits(CreditsAmount);
    Super::HideAndCoolDownPowerUp();
  }
}
