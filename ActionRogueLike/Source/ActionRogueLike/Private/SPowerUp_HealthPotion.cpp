// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerUp_HealthPotion.h"
#include "SAttributeComponent.h"

ASPowerUp_HealthPotion::ASPowerUp_HealthPotion() {
  MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
  MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  MeshComp->SetupAttachment(RootComponent);

  HealAmount = 100.0f;
}

void ASPowerUp_HealthPotion::Interact_Implementation(APawn *InstigatorPawn) {
  if (!ensure(InstigatorPawn)) {
    return;
  }

  USAttributeComponent *AttributeComp = Cast<USAttributeComponent>(
      InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

  if (ensure(AttributeComp) && !(AttributeComp->IsFullHealth())) {
    if (AttributeComp->ApplyHealthChange(this, HealAmount)) {
      Super::HideAndCoolDownPowerUp();
    }
  }
}
