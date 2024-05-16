// Fill out your copyright notice in the Description page of Project Settings.

#include "SAttributeComponent.h"
#include "Net/UnrealNetwork.h"
#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(
    TEXT("su.DamageMultiplier"), 1.0f,
    TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

USAttributeComponent::USAttributeComponent() {
  HealthMax = 100.0f;
  Health = HealthMax;
  Rage = 0.0f;
  RageMax = 100.0f;

  SetIsReplicatedByDefault(true);
}

bool USAttributeComponent::IsAlive() const { return Health > 0.0f; }

bool USAttributeComponent::IsFullHealth() const { return Health == HealthMax; }

float USAttributeComponent::GetHealthMax() const { return HealthMax; }

float USAttributeComponent::GetHealth() { return Health; }

float USAttributeComponent::GetRage() const { return Rage; }

float USAttributeComponent::GetRageMax() const { return RageMax; }

void USAttributeComponent::MulticastHealthChanged_Implementation(
    AActor *InstigatorActor, float NewHealth, float Delta) {
  OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void USAttributeComponent::MulticastRageChanged_Implementation(
    AActor *InstigatorActor, float NewRage, float Delta) {
  OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

bool USAttributeComponent::ApplyHealthChange(AActor *InstigatorActor,
                                             float Delta) {
  if (!GetOwner()->CanBeDamaged() && Delta < 0.0f) {
    return false;
  }

  if (Delta < 0.0f) {
    float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

    Delta *= DamageMultiplier;
  }

  float OldHealth = Health;
  float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
  float ActualDelta = NewHealth - OldHealth;

  if (GetOwner()->HasAuthority()) {
    Health = NewHealth;
    MulticastHealthChanged(InstigatorActor, Health, ActualDelta);

    // Died
    if (ActualDelta < 0.0f && Health == 0.0f) {
      ASGameModeBase *GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
      if (GM) {
        GM->OnActorKilled(GetOwner(), InstigatorActor);
      }
    }
  }

  return ActualDelta != 0;
}

void USAttributeComponent::ApplyRageChange(AActor *InstigatorActor,
                                           float Delta) {
  float OldRage = Rage;

  Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);

  float ActualDelta = OldRage - Rage;

  if (ActualDelta != 0.0f) {
    MulticastRageChanged(InstigatorActor, Rage, ActualDelta);
  }
}

USAttributeComponent *USAttributeComponent::GetAttributes(AActor *FromActor) {
  if (FromActor) {
    return Cast<USAttributeComponent>(
        FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
  }

  return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor *Actor) {
  USAttributeComponent *AttributeComp = GetAttributes(Actor);
  if (AttributeComp) {
    return AttributeComp->IsAlive();
  }

  return false;
}

bool USAttributeComponent::Kill(AActor *InstigatorActor) {
  return ApplyHealthChange(InstigatorActor, -HealthMax);
}

void USAttributeComponent::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(USAttributeComponent, Health);
  DOREPLIFETIME(USAttributeComponent, HealthMax);
  DOREPLIFETIME(USAttributeComponent, Rage);
  DOREPLIFETIME(USAttributeComponent, RageMax);

  // DOREPLIFETIME_CONDITION(USAttributeComponent, HealthMax, COND_InitialOnly);
}
