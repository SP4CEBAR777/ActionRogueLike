// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SAIController.h"

void ASAIController::BeginPlay() {
  Super::BeginPlay();

  if (ensureMsgf(BehaviorTree, TEXT("BehaviorTree not set!"))) {
    RunBehaviorTree(BehaviorTree);
  }
}
