// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter {
  GENERATED_BODY()

public:
  ASAICharacter();

protected:
  virtual void PostInitializeComponents() override;

  UPROPERTY(VisibleAnywhere, Category = "AI")
  UPawnSensingComponent *PawnSensingComp;

  UPROPERTY(VisibleAnywhere, Category = "AI")
  FName TargetActorKey;

  UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
  USAttributeComponent *AttributeComp;

  UFUNCTION()
  void OnPawnSeen(APawn *Pawn);

  UFUNCTION()
  void OnHealthChanged(AActor *InstigatorActor,
                       USAttributeComponent *OwningComp, float NewHealth,
                       float Delta);

  void SetTargetActor(AActor *TargetActor);
};
