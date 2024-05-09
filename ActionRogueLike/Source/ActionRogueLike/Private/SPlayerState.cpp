// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "SSavegame.h"

ASPlayerState::ASPlayerState() { SetReplicates(true); }

bool ASPlayerState::AddCredits(int32 Delta) {
  if (!ensure(Delta > 0.0f)) {
    return false;
  }

  Credits += Delta;

  OnCreditsChanged.Broadcast(this, Credits, Delta);

  return true;
}

bool ASPlayerState::RemoveCredits(int32 Delta) {
  if (!ensure(Delta > 0.0f)) {
    return false;
  }

  if (Credits < Delta) {
    return false;
  }

  Credits -= Delta;

  OnCreditsChanged.Broadcast(this, Credits, -Delta);

  return true;
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame *SaveObject) {
  if (SaveObject) {
    SaveObject->Credits = Credits;
  }
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame *SaveObject) {
  if (SaveObject) {
    // Makes sure we trigger credits changed event
    AddCredits(SaveObject->Credits);

    // Credits = SaveObject->Credits;
  }
}

void ASPlayerState::OnRep_Credits(int32 OldCredits) {
  OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

void ASPlayerState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(ASPlayerState, Credits);
}

int32 ASPlayerState::GetCredits() const { return Credits; }
