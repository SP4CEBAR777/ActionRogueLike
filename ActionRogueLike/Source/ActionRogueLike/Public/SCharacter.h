// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;

UCLASS() class ACTIONROGUELIKE_API ASCharacter : public ACharacter {
  GENERATED_BODY()

public:
  // Sets default values for this character's properties
  ASCharacter();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  UPROPERTY(VisibleAnywhere)
  USpringArmComponent *SpringArmComp;

  UPROPERTY(VisibleAnywhere)
  UCameraComponent *CameraComp;

  UPROPERTY(VisibleAnywhere)
  USInteractionComponent *InteractionComp;

  UPROPERTY(EditAnywhere, Category = "Attack")
  TSubclassOf<class AActor> ProjectileClass;

  UPROPERTY(EditAnywhere, Category = "Attack")
  UAnimMontage *AttackAnim;
  FTimerHandle TimerHandle_PrimaryAttack;

  UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
  USAttributeComponent *AttributeComp;

  void MoveForward(float value);
  void MoveRight(float value);
  void PrimaryAttack();
  void PrimaryAttack_TimeElapsed();
  void PrimaryInteract();

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(
      class UInputComponent *PlayerInputComponent) override;
};
