// Fill out your copyright notice in the Description page of Project Settings.

#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent() {
  // Set this component to be initialized when the game starts, and to be ticked
  // every frame.  You can turn these features off to improve performance if you
  // don't need them.
  PrimaryComponentTick.bCanEverTick = true;

  // ...
}

// Called when the game starts
void USInteractionComponent::BeginPlay() {
  Super::BeginPlay();

  // ...
}

// Called every frame
void USInteractionComponent::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction *ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // ...
}

void USInteractionComponent::PrimaryInteract() {
  FHitResult Hit;
  FVector EyeLocation;
  FRotator EyeRotation;
  AActor *MyOwner = GetOwner();
  MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
  FVector End = EyeLocation + (EyeRotation.Vector() * 1000);
  FCollisionObjectQueryParams ObjectQueryParams;
  ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

  GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End,
                                          ObjectQueryParams);

  AActor *HitActor = Hit.GetActor();
  if (HitActor) {
    if (HitActor->Implements<USGameplayInterface>()) {
      APawn *MyPawn = Cast<APawn>(MyOwner);
      ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
      DrawDebugLine(GetWorld(), EyeLocation, End, FColor::Green, false, 3);
    }
  } else {
    DrawDebugLine(GetWorld(), EyeLocation, End, FColor::Red, false, 3);
  }
}
