// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerUp_HealthPotion.h"
#include "SAttributeComponent.h"

ASPowerUp_HealthPotion::ASPowerUp_HealthPotion() { HealAmount = 100.0f; }

void ASPowerUp_HealthPotion::Interact_Implementation(APawn *InstigatorPawn) {
  USAttributeComponent *AttributeComp = Cast<USAttributeComponent>(
      InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

  if (ensure(AttributeComp) && !(AttributeComp->IsMaxHealth())) {
    if (AttributeComp->ApplyHealthChange(HealAmount)) {
      Super::HideAndCoolDownPowerUp();
    }
  }
}
