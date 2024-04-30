// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerState.h"

bool ASPlayerState::AddCredits(int32 Delta) {
  if (!ensure(Delta > 0)) {
    return false;
  }

  Credits += Delta;

  OnCreditsChanged.Broadcast(this, Credits, Delta);

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

  OnCreditsChanged.Broadcast(this, Credits, Delta);

  return true;
}

int32 ASPlayerState::GetCredits() const { return Credits; }
