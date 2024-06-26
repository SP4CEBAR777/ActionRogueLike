// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;

/**
 *
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction {
  GENERATED_BODY()

protected:
  UPROPERTY(EditAnywhere, Category = "Attack")
  TSubclassOf<AActor> ProjectileClass;

  UPROPERTY(VisibleAnywhere, Category = "Effects")
  FName HandSocketName;

  UPROPERTY(EditAnywhere, Category = "Attack")
  UAnimMontage *AttackAnim;

  UPROPERTY(EditAnywhere, Category = "Attack")
  UParticleSystem *CastingEffect;

  UPROPERTY(EditDefaultsOnly, Category = "Attack")
  float AttackAnimDelay;

  /* Sphere radius of the sweep to find desired target under crosshair. Adjusts
   * final projectile direction */
  UPROPERTY(EditAnywhere, Category = "Targeting")
  float SweepRadius;

  /* Fallback distance when sweep finds no collision under crosshair. Adjusts
   * final projectile direction */
  UPROPERTY(EditAnywhere, Category = "Targeting")
  float SweepDistanceFallback;

  UFUNCTION()
  void AttackDelay_Elapsed(ACharacter *InstigatorCharacter);

public:
  USAction_ProjectileAttack();

  virtual void StartAction_Implementation(AActor *Instigator) override;
};
