// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SBTTask_HealSelf.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type
USBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent &OwnerComp,
                               uint8 *NodeMemory) {
  APawn *AIPawn = OwnerComp.GetAIOwner()->GetPawn();
  if (ensure(AIPawn)) {
    USAttributeComponent *AttributeComp =
        USAttributeComponent::GetAttributes(AIPawn);
    if (ensure(AttributeComp)) {
      AttributeComp->ApplyHealthChange(AIPawn, AttributeComp->GetHealthMax());
      DrawDebugString(GetWorld(), AIPawn->GetActorLocation(), "HEALING SELF",
                      nullptr, FColor::White, 1.0f, true);
      return EBTNodeResult::Succeeded;
    }
  }
  return EBTNodeResult::Failed;
}
