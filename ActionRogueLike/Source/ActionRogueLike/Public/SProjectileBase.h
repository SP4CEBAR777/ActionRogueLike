// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UAudioComponent;
class USoundCue;

UCLASS(
    ABSTRACT) // 'ABSTRACT' marks this class as incomplete, keeping this out of
              // certain dropdowns windows like SpawnActor in Unreal Editor
class ACTIONROGUELIKE_API ASProjectileBase : public AActor {
  GENERATED_BODY()

protected:
  UPROPERTY(EditDefaultsOnly, Category = "Effects")
  UParticleSystem *ImpactVFX;

  UPROPERTY(EditDefaultsOnly, Category = "Effects")
  USoundCue *ImpactSound;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  USphereComponent *SphereComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  UProjectileMovementComponent *MoveComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  UParticleSystemComponent *EffectComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  UAudioComponent *AudioComp;

  UFUNCTION()
  virtual void OnActorHit(UPrimitiveComponent *HitComponent, AActor *OtherActor,
                          UPrimitiveComponent *OtherComp, FVector NormalImpulse,
                          const FHitResult &Hit);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
  void Explode();

  virtual void PostInitializeComponents() override;

public:
  ASProjectileBase();
};
