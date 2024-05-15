// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged,
                                             USActionComponent *, OwningComp,
                                             USAction *, Action);

class USAction;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent {
  GENERATED_BODY()

public:
  USActionComponent();

  UPROPERTY(BlueprintAssignable)
  FOnActionStateChanged OnActionStarted;

  UPROPERTY(BlueprintAssignable)
  FOnActionStateChanged OnActionStopped;

  virtual void
  TickComponent(float DeltaTime, ELevelTick TickType,
                FActorComponentTickFunction *ThisTickFunction) override;

  UFUNCTION(BlueprintCallable, Category = "Action")
  void AddAction(AActor *Instigator, TSubclassOf<USAction> ActionClass);

  UFUNCTION(BlueprintCallable, Category = "Action")
  void RemoveAction(USAction *ActionClass);

  virtual bool ReplicateSubobjects(class UActorChannel *Channel,
                                   class FOutBunch *Bunch,
                                   FReplicationFlags *RepFlags) override;

  UFUNCTION(BlueprintCallable, Category = "Action")
  bool StartActionByName(AActor *Instigator, FName ActionName);

  UFUNCTION(BlueprintCallable, Category = "Action")
  bool StopActionByName(AActor *Instigator, FName ActionName);

  UFUNCTION(BlueprintCallable, Category = "Action")
  USAction *GetAction(TSubclassOf<USAction> ActionToFind) const;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
  FGameplayTagContainer ActiveGameplayTags;

protected:
  virtual void BeginPlay() override;

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  UFUNCTION(Server, Reliable)
  void ServerStartActionByName(AActor *Instigator, FName ActionName);

  UFUNCTION(Server, Reliable)
  void ServerStopActionByName(AActor *Instigator, FName ActionName);

  UPROPERTY(EditAnywhere, Category = "Actions")
  TArray<TSubclassOf<USAction>> DefaultActions;

  UPROPERTY(BlueprintReadOnly, Replicated)
  TArray<USAction *> Actions;
};
