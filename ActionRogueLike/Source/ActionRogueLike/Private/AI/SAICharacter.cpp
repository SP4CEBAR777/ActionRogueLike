// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SAICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"

ASAICharacter::ASAICharacter() {
  PawnSensingComp =
      CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

  AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

  TargetActorKey = "TargetActor";

  TimeToHitParamName = "TimeToHit";

  AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

  GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic,
                                                       ECR_Ignore);
  GetMesh()->SetGenerateOverlapEvents(true);
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
    if (InstigatorActor != this) {
      SetTargetActor(InstigatorActor);
    }

    GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName,
                                                  GetWorld()->TimeSeconds);

    if (ActiveHealthBar == nullptr) {
      ActiveHealthBar =
          CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
      if (ActiveHealthBar) {
        ActiveHealthBar->AttachedActor = this;
        ActiveHealthBar->AddToViewport();
      }
    }

    // Killed
    if (NewHealth <= 0.0f) {
      // stop BT
      AAIController *AIC = Cast<AAIController>(GetController());
      if (AIC) {
        AIC->GetBrainComponent()->StopLogic("Killed");
      }

      // ragdoll
      GetMesh()->SetAllBodiesSimulatePhysics(true);
      GetMesh()->SetCollisionProfileName("Ragdoll");

      GetCapsuleComponent()->SetCollisionEnabled(
          ECollisionEnabled::NoCollision);
      GetCharacterMovement()->DisableMovement();

      // set lifespan
      SetLifeSpan(10.0f);
    }
  }
}

void ASAICharacter::SetTargetActor(AActor *NewTarget) {
  AAIController *AIC = Cast<AAIController>(GetController());
  if (AIC) {
    AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
  }
}
