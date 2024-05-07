// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"

ASPlayerState::ASPlayerState() { SetReplicates(true); }

bool ASPlayerState::AddCredits(int32 Delta) {
  if (!ensure(Delta > 0)) {
    return false;
  }

  Credits += Delta;

  MulticastCreditsChanged(this, Credits, Delta);

  return true;
}

bool ASPlayerState::RemoveCredits(int32 Delta) {
  if (!ensure(Delta > 0)) {
    return false;
  }

  if (Credits < Delta) {
    return false;
  }

  Credits -= Delta;

  MulticastCreditsChanged(this, Credits, Delta);

  return true;
}

int32 ASPlayerState::GetCredits() const { return Credits; }

void ASPlayerState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(ASPlayerState, Credits);
}

void ASPlayerState::MulticastCreditsChanged_Implementation(
    ASPlayerState *InstigatorActor, int32 NewCredits, int32 Delta) {
  OnCreditsChanged.Broadcast(this, NewCredits, Delta);
}
