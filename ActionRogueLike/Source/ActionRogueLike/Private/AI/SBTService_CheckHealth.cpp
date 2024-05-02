// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SBTService_CheckHealth.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributeComponent.h"

USBTService_CheckHealth::USBTService_CheckHealth() { LowHealthFraction = 0.3f; }

void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent &OwnerComp,
                                       uint8 *NodeMemory, float DeltaSeconds) {
  Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

  UBlackboardComponent *BlackboardComp = OwnerComp.GetBlackboardComponent();
  if (ensure(BlackboardComp)) {
    APawn *AIPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (ensure(AIPawn)) {
      USAttributeComponent *AttributeComp =
          USAttributeComponent::GetAttributes(AIPawn);
      float CurrHealth = AttributeComp->GetHealth();
      float MaxHealth = AttributeComp->GetHealthMax();
      if (CurrHealth / MaxHealth <= LowHealthFraction) {
        BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, true);
      }
    }
  }
}
