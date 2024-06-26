// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTService.h"
#include "CoreMinimal.h"
#include "SBTService_CheckHealth.generated.h"

/**
 *
 */
UCLASS()
class ACTIONROGUELIKE_API USBTService_CheckHealth : public UBTService {
  GENERATED_BODY()

public:
  USBTService_CheckHealth();

private:
  UPROPERTY(EditAnywhere, Category = "AI")
  FBlackboardKeySelector LowHealthKey;

  float LowHealthFraction;

  virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory,
                        float DeltaSeconds) override;
};
