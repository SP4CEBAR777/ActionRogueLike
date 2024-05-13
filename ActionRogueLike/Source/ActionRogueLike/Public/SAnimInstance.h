// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "CoreMinimal.h"
#include "SAnimInstance.generated.h"

class USActionComponent;

/**
 *
 */
UCLASS()
class ACTIONROGUELIKE_API USAnimInstance : public UAnimInstance {
  GENERATED_BODY()

protected:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
  bool bIsStunned;

  UPROPERTY(BlueprintReadOnly, Category = "Animation")
  USActionComponent *ActionComp;

  void NativeInitializeAnimation() override;

  void NativeUpdateAnimation(float DeltaSeconds) override;
};
