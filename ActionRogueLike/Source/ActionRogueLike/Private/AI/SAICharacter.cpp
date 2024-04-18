// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SAICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Perception/PawnSensingComponent.h"

ASAICharacter::ASAICharacter() {
  PawnSensingComp =
      CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

  TargetActorName = "TargetActor";
}

void ASAICharacter::PostInitializeComponents() {
  Super::PostInitializeComponents();

  PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
}

void ASAICharacter::OnPawnSeen(APawn *Pawn) {
  AAIController *AIC = Cast<AAIController>(GetController());
  if (AIC) {
    UBlackboardComponent *BBComp = AIC->GetBlackboardComponent();
    BBComp->SetValueAsObject(TargetActorName, Pawn);

    DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr,
                    FColor::White, 1.0f);
  }
}
