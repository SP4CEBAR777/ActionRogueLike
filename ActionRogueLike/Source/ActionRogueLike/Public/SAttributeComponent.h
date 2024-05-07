// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor *,
                                              InstigatorActor,
                                              USAttributeComponent *,
                                              OwningComp, float, NewHealth,
                                              float, Delta);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor *,
                                              InstigatorActor,
                                              USAttributeComponent *,
                                              OwningComp, float, NewValue,
                                              float, Delta);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent {
  GENERATED_BODY()

public:
  USAttributeComponent();

  UFUNCTION(BlueprintCallable, Category = "Attributes")
  static USAttributeComponent *GetAttributes(AActor *FromActor);

  UPROPERTY(BlueprintAssignable)
  FOnAttributeChanged OnHealthChanged;

  UPROPERTY(BlueprintAssignable)
  FOnAttributeChanged OnRageChanged;

  bool Kill(AActor *InstigatorActor);

  UFUNCTION(BlueprintCallable, Category = "Attributes")
  bool ApplyHealthChange(AActor *InstigatorActor, float Delta);

  UFUNCTION(BlueprintCallable, Category = "Attributes")
  void ApplyRageChange(AActor *InstigatorActor, float Delta);

  UFUNCTION(BlueprintCallable, Category = "Attributes",
            meta = (DisplayName = "IsAlive"))
  static bool IsActorAlive(AActor *Actor);

  UFUNCTION(BlueprintCallable)
  bool IsAlive() const;

  UFUNCTION(BlueprintCallable)
  bool IsFullHealth() const;

  UFUNCTION(BlueprintCallable)
  float GetHealthMax() const;

  UFUNCTION(BlueprintCallable)
  float GetHealth();

  UFUNCTION(BlueprintCallable)
  float GetRage() const;

  UFUNCTION(BlueprintCallable)
  float GetRageMax() const;

protected:
  // EditAnywhere - edit in BP editor and per-instance in level.
  // VisibleAnywhere - 'read-only' in editor and level. (Use for Components)
  // EditDefaultsOnly - hide variable per-instance, edit in BP editor only
  // VisibleDefaultsOnly - 'read-only' access for variable, only in BP editor
  // (uncommon) EditInstanceOnly - allow only editing of instance (eg. when
  // placed in level)
  // --
  // BlueprintReadOnly - read-only in the Blueprint scripting (does not affect
  // 'details'-panel) BlueprintReadWrite - read-write access in Blueprints
  // --
  // Category = "" - display only for detail panels and blueprint context menu.

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated,
            Category = "Attributes")
  float Health;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated,
            Category = "Attributes")
  float HealthMax;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated,
            Category = "Attributes")
  float Rage;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated,
            Category = "Attributes")
  float RageMax;

  UFUNCTION(NetMulticast, Unreliable)
  void MulticastHealthChanged(AActor *InstigatorActor, float NewHealth,
                              float Delta);

  UFUNCTION(NetMulticast, Unreliable)
  void MulticastRageChanged(AActor *InstigatorActor, float NewRage,
                            float Delta);
};
