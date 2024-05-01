#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SProjectileBase.h"
#include "SMagicProjectile.generated.h"

class USActionEffect;

UCLASS() class ACTIONROGUELIKE_API ASMagicProjectile : public ASProjectileBase {
  GENERATED_BODY()

protected:
  UPROPERTY(EditDefaultsOnly, Category = "Damage")
  float DamageAmount;

  UPROPERTY(EditDefaultsOnly, Category = "Damage")
  TSubclassOf<USActionEffect> BurningActionClass;

  UPROPERTY(EditDefaultsOnly, Category = "Tag") FGameplayTag ParryTag;

  UFUNCTION()
  void OnActorOverlap(UPrimitiveComponent *OverlappedComponent,
                      AActor *OtherActor, UPrimitiveComponent *OtherComp,
                      int32 OtherBodyIndex, bool bFromSweep,
                      const FHitResult &SweepResult);

public:
  ASMagicProjectile();
};
