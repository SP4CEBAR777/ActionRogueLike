// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "SSavegame.h"

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

void ASPlayerState::SavePlayerState_Implementation(USSaveGame *SaveObject) {
  if (SaveObject) {
    FString DebugMsg =
        FString::Printf(TEXT("SaveObject name: %s"), *GetNameSafe(SaveObject));
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, DebugMsg);
    SaveObject->Credits = Credits;
  }
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame *SaveObject) {
  if (SaveObject) {
    Credits = SaveObject->Credits;
  }
}

void ASPlayerState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(ASPlayerState, Credits);
}

void ASPlayerState::MulticastCreditsChanged_Implementation(
    ASPlayerState *InstigatorActor, int32 NewCredits, int32 Delta) {
  OnCreditsChanged.Broadcast(this, NewCredits, Delta);
}
