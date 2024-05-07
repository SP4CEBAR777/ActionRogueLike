// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;
class USWorldUserWidget;
class USActionComponent;
class USoundBase;

UCLASS() class ACTIONROGUELIKE_API ASAICharacter : public ACharacter {
  GENERATED_BODY()

public:
  ASAICharacter();

protected:
  UPROPERTY(VisibleAnywhere, Category = "AI")
  USWorldUserWidget *ActiveHealthBar;

  UPROPERTY(EditDefaultsOnly, Category = "AI")
  USoundBase *EnemySpottedSound;

  UPROPERTY(VisibleAnywhere, Category = "Components")
  UPawnSensingComponent *PawnSensingComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  USAttributeComponent *AttributeComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  USActionComponent *ActionComp;

  UPROPERTY(EditDefaultsOnly, Category = "UI")
  TSubclassOf<UUserWidget> HealthBarWidgetClass;

  UPROPERTY(EditDefaultsOnly, Category = "UI")
  TSubclassOf<USWorldUserWidget> EnemySpottedWidgetClass;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  FName TargetActorKey;

  UPROPERTY(VisibleAnywhere, Category = "Effects")
  FName TimeToHitParamName;

  void SetTargetActor(AActor *NewTarget);

  virtual void PostInitializeComponents() override;

  UFUNCTION()
  void OnHealthChanged(AActor *InstigatorActor,
                       USAttributeComponent *OwningComp, float NewHealth,
                       float Delta);

  UFUNCTION(BlueprintCallable, Category = "AI")
  AActor *GetTargetActor() const;

  UFUNCTION()
  void OnPawnSeen(APawn *Pawn);

  UFUNCTION(NetMulticast, Unreliable)
  void MulticastOnPawnSeen(APawn *Pawn);
};
