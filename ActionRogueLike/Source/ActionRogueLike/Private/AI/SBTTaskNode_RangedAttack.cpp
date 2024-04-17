// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SBTTaskNode_RangedAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type
USBTTaskNode_RamgedAttack::ExecuteTask(UBehaviorTreeComponent &OwnerComp,
                                       uint8 *NodeMemory) {
  AAIController *MyController = OwnerComp.GetAIOwner();
  if (ensure(MyController)) {
    ACharacter *MyPawn = Cast<ACharacter>(MyController->GetPawn());
    if (MyPawn == nullptr) {
      return EBTNodeResult::Failed;
    }

    AActor *TargetActor = Cast<AActor>(
        OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
    if (TargetActor == nullptr) {
      return EBTNodeResult::Failed;
    }

    FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");
    FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
    FRotator MuzzleRotation = Direction.Rotation();

    FActorSpawnParameters params;
    params.Instigator = MyPawn;
    params.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ensure(ProjectileClass);
    AActor *NewProj = GetWorld()->SpawnActor<AActor>(
        ProjectileClass, MuzzleLocation, MuzzleRotation);
    return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
  }

  return EBTNodeResult::Type();
}
