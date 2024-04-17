// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"
#include "SBTTaskNode_RangedAttack.generated.h"

/**
 *
 */
UCLASS()
class ACTIONROGUELIKE_API USBTTaskNode_RamgedAttack : public UBTTaskNode {
  GENERATED_BODY()

  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp,
                                          uint8 *NodeMemory) override;

protected:
  UPROPERTY(EditAnywhere, Category = "AI")
  TSubclassOf<AActor> ProjectileClass;
};
