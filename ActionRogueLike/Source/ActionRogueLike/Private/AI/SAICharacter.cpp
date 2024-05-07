// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SAICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Perception/PawnSensingComponent.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"

ASAICharacter::ASAICharacter() {
  PawnSensingComp =
      CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

  AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

  ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

  TargetActorKey = "TargetActor";

  TimeToHitParamName = "TimeToHit";

  AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

  GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic,
                                                       ECR_Ignore);
  GetMesh()->SetGenerateOverlapEvents(true);

  SetReplicates(true);
}

void ASAICharacter::PostInitializeComponents() {
  Super::PostInitializeComponents();

  PawnSensingComp->OnSeePawn.AddDynamic(this,
                                        &ASAICharacter::MulticastOnPawnSeen);
  AttributeComp->OnHealthChanged.AddDynamic(this,
                                            &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnPawnSeen(APawn *Pawn) {
  if (Pawn != GetTargetActor()) {
    SetTargetActor(Pawn);

    MulticastOnPawnSeen(Pawn);
  }
}

void ASAICharacter::MulticastOnPawnSeen_Implementation(APawn *Pawn) {
  USWorldUserWidget *NewWidget =
      CreateWidget<USWorldUserWidget>(GetWorld(), EnemySpottedWidgetClass);
  if (NewWidget) {
    NewWidget->AttachedActor = this;
    // Index of 10 (or anything higher than default of 0) places this on top of
    // any other widget. May end up behind the minion health bar otherwise.
    NewWidget->AddToViewport(10);
  }
}

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

AActor *ASAICharacter::GetTargetActor() const {
  AAIController *AIC = Cast<AAIController>(GetController());
  if (AIC) {
    AActor *MyActor = Cast<AActor>(
        AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
    if (MyActor) {
      return MyActor;
    }
  }
  return nullptr;
}

void ASAICharacter::SetTargetActor(AActor *NewTarget) {
  AAIController *AIC = Cast<AAIController>(GetController());
  if (AIC) {
    AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
  }
}
