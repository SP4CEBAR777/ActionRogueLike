// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "SInteractionComponent.generated.h"

class USWorldUserWidget;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent {
  GENERATED_BODY()

public:
  USInteractionComponent();

  virtual void
  TickComponent(float DeltaTime, ELevelTick TickType,
                FActorComponentTickFunction *ThisTickFunction) override;

  void PrimaryInteract();

protected:
  UPROPERTY(EditDefaultsOnly, Category = "Trace")
  float TraceDistance;

  UPROPERTY(EditDefaultsOnly, Category = "Trace")
  float TraceRadius;

  UPROPERTY(EditDefaultsOnly, Category = "Trace")
  TEnumAsByte<ECollisionChannel> CollisionChannel;

  UPROPERTY() AActor *FocusActor;

  UPROPERTY(EditDefaultsOnly, Category = "UI")
  TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

  UPROPERTY()
  USWorldUserWidget *DefaultWidgetInstance;

  void FindBestInteractable();

  UFUNCTION(Server, Reliable)
  void ServerInteract(AActor *InFocus);
};
