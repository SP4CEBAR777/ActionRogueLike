// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

USTRUCT()
struct FActionRepData {
  GENERATED_BODY()

public:
  UPROPERTY()
  bool bIsRunning;

  UPROPERTY()
  AActor *Instigator;
};

/**
 *
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject {
  GENERATED_BODY()

protected:
  UPROPERTY(Replicated)
  USActionComponent *ActionComp;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
  UTexture2D *Icon;

  UPROPERTY(EditDefaultsOnly, Category = "Tags")
  FGameplayTagContainer GrantsTags;

  UPROPERTY(EditDefaultsOnly, Category = "Tags")
  FGameplayTagContainer BlockedTags;

  UFUNCTION(BlueprintCallable, Category = "Action")
  USActionComponent *GetOwningComponent() const;

  UPROPERTY(ReplicatedUsing = "OnRep_RepData")
  FActionRepData RepData;

  UPROPERTY(Replicated)
  float TimeStarted;

  UFUNCTION()
  void OnRep_RepData();

public:
  void Initialize(USActionComponent *NewActionComp);

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

  virtual bool IsSupportedForNetworking() const override { return true; }
};
