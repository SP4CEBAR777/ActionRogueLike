// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "SAttributeComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent {
  GENERATED_BODY()

public:
  // Sets default values for this component's properties
  USAttributeComponent();

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
  float Health;

public:
  UFUNCTION(BlueprintCallable, Category = "Attributes")
  bool ApplyHealthChange(float Delta);
};
