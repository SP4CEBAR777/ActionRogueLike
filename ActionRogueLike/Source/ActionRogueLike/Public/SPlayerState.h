// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged,
                                               ASPlayerState *, PlayerState,
                                               int32, NewCredits, int32, Delta);

/**
 *
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState {
  GENERATED_BODY()

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Credits",
            Replicated)
  int32 Credits;

  UFUNCTION(NetMulticast, Reliable)
  void MulticastCreditsChanged(ASPlayerState *InstigatorActor, int32 NewCredits,
                               int32 Delta);

public:
  ASPlayerState();

  UFUNCTION(BlueprintCallable, Category = "Credits")
  bool AddCredits(int32 Delta);

  UFUNCTION(BlueprintCallable, Category = "Credits")
  bool RemoveCredits(int32 Delta);

  UFUNCTION(BlueprintCallable, Category = "Credits")
  int32 GetCredits() const;

  UPROPERTY(BlueprintAssignable)
  FOnCreditsChanged OnCreditsChanged;
};
