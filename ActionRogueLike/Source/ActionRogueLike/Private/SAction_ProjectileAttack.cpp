// Fill out your copyright notice in the Description page of Project Settings.

#include "SAction_ProjectileAttack.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

USAction_ProjectileAttack::USAction_ProjectileAttack() {

  HandSocketName = "Muzzle_01";

  AttackAnimDelay = 0.2f;

  SweepRadius = 20.0f;
  SweepDistanceFallback = 5000;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor *Instigator) {
  Super::StartAction_Implementation(Instigator);

  ACharacter *MyCharacter = Cast<ACharacter>(Instigator);
  if (MyCharacter) {
    MyCharacter->PlayAnimMontage(AttackAnim);

    UGameplayStatics::SpawnEmitterAttached(
        CastingEffect, MyCharacter->GetMesh(), HandSocketName,
        FVector::ZeroVector, FRotator::ZeroRotator,
        EAttachLocation::SnapToTarget);
  }

  if (MyCharacter->HasAuthority()) {
    FTimerHandle TimerHandle_AttackDelay;
    FTimerDelegate Delegate;
    Delegate.BindUFunction(this, "AttackDelay_Elapsed", MyCharacter);

    GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate,
                                           AttackAnimDelay, false);
  }
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(
    ACharacter *InstigatorCharacter) {
  if (ensureAlways(ProjectileClass)) {
    FVector HandLocation =
        InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Instigator = InstigatorCharacter;

    FCollisionShape Shape;
    Shape.SetSphere(SweepRadius);

    // Ignore Player
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(InstigatorCharacter);

    FVector TraceDirection = InstigatorCharacter->GetControlRotation().Vector();

    // Add sweep radius onto start to avoid the sphere clipping into floor/walls
    // the camera is directly against.
    FVector TraceStart = InstigatorCharacter->GetPawnViewLocation() +
                         (TraceDirection * SweepRadius);

    // endpoint far into the look-at distance (not too far, still adjust
    // somewhat towards crosshair on a miss)
    FVector TraceEnd = TraceStart + (TraceDirection * SweepDistanceFallback);

    FHitResult Hit;
    // returns true if we got to a blocking hit
    if (GetWorld()->SweepSingleByChannel(Hit, TraceStart, TraceEnd,
                                         FQuat::Identity, ECC_GameTraceChannel1,
                                         Shape, Params)) {
      // Overwrite trace end with impact point in world
      TraceEnd = Hit.ImpactPoint;
    }

    // find new direction/rotation from Hand pointing to impact point in world.
    FRotator ProjRotation = (TraceEnd - HandLocation).Rotation();

    FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
    GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
  }

  StopAction(InstigatorCharacter);
}
