#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASProjectileBase {
  GENERATED_BODY()

protected:
  UPROPERTY(EditDefaultsOnly, Category = "Damage")
  float DamageAmount;

  UFUNCTION()
  void OnActorOverlap(UPrimitiveComponent *OverlappedComponent,
                      AActor *OtherActor, UPrimitiveComponent *OtherComp,
                      int32 OtherBodyIndex, bool bFromSweep,
                      const FHitResult &SweepResult);

public:
  ASMagicProjectile();
};
