// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerUp_HealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

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

FText ASPowerUp_HealthPotion::GetInteractText_Implementation(
    APawn *InstigatorPawn) {
  USAttributeComponent *AttributeComp =
      USAttributeComponent::GetAttributes(InstigatorPawn);
  if (AttributeComp && AttributeComp->IsFullHealth()) {
    return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
  }
  return FText::Format(
      LOCTEXT("HealthPotion_InteractMessage",
              "Costs {0} Credits. Restores health to maximum."),
      CreditCost);
}

#undef LOCTEXT_NAMESPACE
