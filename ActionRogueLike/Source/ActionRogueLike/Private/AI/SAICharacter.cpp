// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SAICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "Perception/PawnSensingComponent.h"
#include "SAttributeComponent.h"

ASAICharacter::ASAICharacter() {
  PawnSensingComp =
      CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

  AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

  TargetActorKey = "TargetActor";

  AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ASAICharacter::PostInitializeComponents() {
  Super::PostInitializeComponents();

  PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
  AttributeComp->OnHealthChanged.AddDynamic(this,
                                            &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnPawnSeen(APawn *Pawn) { SetTargetActor(Pawn); }

void ASAICharacter::OnHealthChanged(AActor *InstigatorActor,
                                    USAttributeComponent *OwningComp,
                                    float NewHealth, float Delta) {
  if (Delta < 0.0f) {
    if (NewHealth <= 0.0f) {
      // stop BT
      AAIController *AIC = Cast<AAIController>(GetController());
      if (AIC) {
        AIC->GetBrainComponent()->StopLogic("Killed");
      }

      // ragdoll
      GetMesh()->SetAllBodiesSimulatePhysics(true);
      GetMesh()->SetCollisionProfileName("Ragdoll");

      // set lifespan
      SetLifeSpan(10.0f);

      return;
    }

    if (InstigatorActor != this) {
      SetTargetActor(InstigatorActor);
    }
  }
}

void ASAICharacter::SetTargetActor(AActor *NewTarget) {
  AAIController *AIC = Cast<AAIController>(GetController());
  if (AIC) {
    AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
  }
}
