// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SActionComponent.h"
#include "SActionEffect.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"

ASMagicProjectile::ASMagicProjectile() {
  SphereComp->SetSphereRadius(20.0f);

  InitialLifeSpan = 10.0f;

  DamageAmount = 20.0f;
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent *OverlappedComponent,
                                       AActor *OtherActor,
                                       UPrimitiveComponent *OtherComp,
                                       int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult &SweepResult) {
  if (OtherActor && OtherActor != GetInstigator()) {
    USActionComponent *ActionComp = Cast<USActionComponent>(
        OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
    // Parry
    if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag)) {
      MoveComp->Velocity = -MoveComp->Velocity;

      SetInstigator(Cast<APawn>(OtherActor));
      return;
    }

    // Damge & Explode
    if (USGameplayFunctionLibrary::ApplyDirectionalDamage(
            GetInstigator(), OtherActor, DamageAmount, SweepResult)) {
      Super::Explode();

      if (ActionComp && BurningActionClass && HasAuthority()) {
        ActionComp->AddAction(GetInstigator(), BurningActionClass);
      }
    }
  }
}

void ASMagicProjectile::PostInitializeComponents() {
  Super::PostInitializeComponents();

  SphereComp->OnComponentBeginOverlap.AddDynamic(
      this, &ASMagicProjectile::OnActorOverlap);
}
