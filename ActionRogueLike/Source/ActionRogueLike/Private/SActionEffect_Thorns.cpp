// Fill out your copyright notice in the Description page of Project Settings.

#include "SActionEffect_Thorns.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"

USActionEffect_Thorns::USActionEffect_Thorns() {
  Duration = 0.0f;
  Period = 0.0f;
  ReflectionFraction = 0.2f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor *Instigator) {
  Super::StartAction_Implementation(Instigator);

  USAttributeComponent *AttributeComp =
      USAttributeComponent::GetAttributes(Instigator);
  if (AttributeComp) {
    AttributeComp->OnHealthChanged.AddDynamic(
        this, &USActionEffect_Thorns::OnHealthChanged);
  }
}

void USActionEffect_Thorns::StopAction_Implementation(AActor *Instigator) {
  Super::StopAction_Implementation(Instigator);
  USAttributeComponent *AttributeComp =
      USAttributeComponent::GetAttributes(Instigator);
  if (AttributeComp) {
    AttributeComp->OnHealthChanged.RemoveDynamic(
        this, &USActionEffect_Thorns::OnHealthChanged);
  }
}

void USActionEffect_Thorns::OnHealthChanged(AActor *InstigatorActor,
                                            USAttributeComponent *OwningComp,
                                            float NewHealth, float Delta) {
  AActor *OwningActor = GetOwningComponent()->GetOwner();
  if (Delta < 0.0f && OwningActor != InstigatorActor) {
    int32 ReflectionDamage = FMath::RoundToInt(Delta * ReflectionFraction);
    ReflectionDamage = FMath::Abs(ReflectionDamage);

    if (ReflectionDamage == 0) {
      return;
    }

    USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor,
                                           ReflectionDamage);
  }
}
