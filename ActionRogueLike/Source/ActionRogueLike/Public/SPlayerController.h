// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn *, NewPawn);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged,
                                            APlayerState *, NewPlayerState);

class USPlayerState;

/**
 *
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerController : public APlayerController {
  GENERATED_BODY()

protected:
  UPROPERTY(BlueprintAssignable)
  FOnPawnChanged OnPawnChanged;

  // Listen for incoming player state (for clients this may be nullptr when
  // initially joining a game, afterwards player state will not change again as
  // PlayerControllers maintain the same player state throughout the level)
  UPROPERTY(BlueprintAssignable)
  FOnPlayerStateChanged OnPlayerStateReceived;

  virtual void SetPawn(APawn *InPawn) override;

  void OnRep_PlayerState() override;
};
