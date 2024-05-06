// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"
#include "UObject/NoExportTypes.h"

/**
 *
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject {
  GENERATED_BODY()

protected:
  UPROPERTY(EditDefaultsOnly, Category = "Tags")
  FGameplayTagContainer GrantsTags;

  UPROPERTY(EditDefaultsOnly, Category = "Tags")
  FGameplayTagContainer BlockedTags;

  UFUNCTION(BlueprintCallable, Category = "Action")
  USActionComponent *GetOwningComponent() const;

  UPROPERTY(ReplicateUsing = "OnRep_IsRunning")
  bool bIsRunning;

public:
  UPROPERTY(EditDefaultsOnly, Category = "Action")
  FName ActionName;

  UPROPERTY(EditDefaultsOnly, Category = "Action")
  bool bIsAutoStart;

  UFUNCTION(BlueprintNativeEvent, Category = "Action")
  bool CanStart(AActor *Instigator);

  UFUNCTION(BlueprintNativeEvent, Category = "Action")
  void StartAction(AActor *Instigator);

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
  void StopAction(AActor *Instigator);

  UFUNCTION(BlueprintCallable, Category = "Action")
  bool IsRunning() const;

  UWorld *GetWorld() const override;
};
