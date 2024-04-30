// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerUp_HealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASPowerUp_HealthPotion::ASPowerUp_HealthPotion() {
  HealAmount = 100.0f;
  CreditCost = 50;
}

void ASPowerUp_HealthPotion::Interact_Implementation(APawn *InstigatorPawn) {
  if (!ensure(InstigatorPawn)) {
    return;
  }

  USAttributeComponent *AttributeComp =
      USAttributeComponent::GetAttributes(InstigatorPawn);

  if (ensure(AttributeComp) && !(AttributeComp->IsFullHealth())) {
    if (ASPlayerState *PS = InstigatorPawn->GetPlayerState<ASPlayerState>()) {
      if (PS->RemoveCredits(CreditCost) &&
          AttributeComp->ApplyHealthChange(this, HealAmount)) {
        Super::HideAndCoolDownPowerUp();
      }
    }
  }
}
