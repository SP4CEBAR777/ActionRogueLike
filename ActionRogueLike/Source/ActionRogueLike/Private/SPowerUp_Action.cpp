// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerUp_Action.h"
#include "SAction.h"
#include "SActionComponent.h"

void ASPowerUp_Action::Interact_Implementation(APawn *InstigatorPawn) {
  if (ensure(InstigatorPawn)) {
    USActionComponent *ActionComp = Cast<USActionComponent>(
        InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass()));
    if (ActionComp) {
      if (ActionComp->GetAction(ActionToGrant)) {

        GEngine->AddOnScreenDebugMessage(
            -1, 1.0f, FColor::Red,
            FString::Printf(TEXT("You already have this action: %s"),
                            *GetNameSafe(ActionToGrant)));
        return;
      }
      ActionComp->AddAction(InstigatorPawn, ActionToGrant);
    }
  }
}
