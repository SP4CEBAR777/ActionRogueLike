// Fill out your copyright notice in the Description page of Project Settings.

#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"
#include "SWorldUserWidget.h"

USInteractionComponent::USInteractionComponent() {
  PrimaryComponentTick.bCanEverTick = true;

  TraceDistance = 500.0f;
  TraceRadius = 30.0f;
  CollisionChannel = ECC_WorldDynamic;
}

static TAutoConsoleVariable<bool>
    CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false,
                             TEXT("Enable Debug Lines for Interact Component."),
                             ECVF_Cheat);

void USInteractionComponent::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction *ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  FindBestInteractable();
}

void USInteractionComponent::PrimaryInteract() {
  if (FocusActor == nullptr) {
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red,
                                     "No Focus Actor to Interact");
    return;
  }

  APawn *MyPawn = Cast<APawn>(GetOwner());

  ISGameplayInterface::Execute_Interact(FocusActor, MyPawn);
}

void USInteractionComponent::FindBestInteractable() {
  bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

  FCollisionObjectQueryParams ObjectQueryParams;
  ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

  AActor *MyOwner = GetOwner();

  FVector EyeLocation;
  FRotator EyeRotation;
  MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

  FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

  TArray<FHitResult> Hits;

  FCollisionShape Shape;
  Shape.SetSphere(TraceRadius);

  bool bBlockingHit = GetWorld()->SweepMultiByObjectType(
      Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

  FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

  FocusActor = nullptr;

  for (FHitResult Hit : Hits) {
    if (bDebugDraw) {
      DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor,
                      false, 2.0f);
    }

    AActor *HitActor = Hit.GetActor();
    if (HitActor) {
      if (HitActor->Implements<USGameplayInterface>()) {
        FocusActor = HitActor;
        break;
      }
    }
  }

  if (FocusActor) {
    if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass)) {
      DefaultWidgetInstance =
          CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
    }

    if (DefaultWidgetInstance) {
      DefaultWidgetInstance->AttachedActor = FocusActor;

      if (!DefaultWidgetInstance->IsInViewport()) {
        DefaultWidgetInstance->AddToViewport();
      }
    }
  } else {
    if (DefaultWidgetInstance) {
      DefaultWidgetInstance->RemoveFromParent();
    }
  }

  if (bDebugDraw) {
    DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0,
                  2.0f);
  }
}
