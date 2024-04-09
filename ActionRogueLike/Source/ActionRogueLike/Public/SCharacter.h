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
class UParticleSystem;

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

  UPROPERTY(EditDefaultsOnly, Category = "Attack")
  float AttackAnimDelay;

  UPROPERTY(EditAnywhere, Category = "Attack")
  TSubclassOf<AActor> ProjectileClass;

  UPROPERTY(EditAnywhere, Category = "Attack")
  TSubclassOf<AActor> BlackHoleProjectileCalss;

  UPROPERTY(EditAnywhere, Category = "Attack")
  TSubclassOf<AActor> DashProjectileClass;

  UPROPERTY(EditAnywhere, Category = "Attack")
  UAnimMontage *AttackAnim;

  FTimerHandle TimerHandle_PrimaryAttack;

  UPROPERTY(EditDefaultsOnly, Category = "Attack")
  UParticleSystem *CastingEffect;

  UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
  USAttributeComponent *AttributeComp;

  UPROPERTY(VisibleAnywhere, Category = "Effects")
  FName HandSocketName;

  void MoveForward(float value);
  void MoveRight(float value);
  void PrimaryAttack();
  void PrimaryAttack_TimeElapsed();
  void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);
  void BlackHoleAttack();
  void BlackHoleAttack_TimeElapsed();
  void Dash();
  void Dash_TimeElapsed();
  void PrimaryInteract();
  void StartAttackEffect();
  UFUNCTION()
  void OnHealthChanged(AActor *InstigatorActor,
                       USAttributeComponent *OwningComp, float NewHealth,
                       float Delta);

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(
      class UInputComponent *PlayerInputComponent) override;
};
