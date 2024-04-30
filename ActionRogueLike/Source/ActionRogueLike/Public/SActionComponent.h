// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.generated.h"

class USAction;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent {
  GENERATED_BODY()

public:
  USActionComponent();

  UFUNCTION(BlueprintCallable, Category = "Action")
  void AddAction(TSubclassOf<USAction> ActionClass);

  UFUNCTION(BlueprintCallable, Category = "Action")
  bool StartActionByName(AActor *Instigator, FName ActionName);

  UFUNCTION(BlueprintCallable, Category = "Action")
  bool StopActionByName(AActor *Instigator, FName ActionName);

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
  FGameplayTagContainer ActiveGameplayTags;

protected:
  virtual void BeginPlay() override;

  UPROPERTY(EditAnywhere, Category = "Actions")
  TArray<TSubclassOf<USAction>> DefaultActions;

  UPROPERTY()
  TArray<USAction *> Actions;

public:
  virtual void
  TickComponent(float DeltaTime, ELevelTick TickType,
                FActorComponentTickFunction *ThisTickFunction) override;
};