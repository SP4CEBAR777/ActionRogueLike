// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class AController;
class USSaveGame;

/**
 *
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase {
  GENERATED_BODY()

protected:
  FString SlotName;

  UPROPERTY()
  USSaveGame *CurrentSaveGame;

  UPROPERTY(EditDefaultsOnly, Category = "AI")
  TSubclassOf<AActor> MinionClass;

  UPROPERTY(EditDefaultsOnly, Category = "AI")
  TArray<TSubclassOf<AActor>> PowerupClasses;

  UPROPERTY(EditDefaultsOnly, Category = "AI")
  UEnvQuery *SpawnBotQuery;

  UPROPERTY(EditDefaultsOnly, Category = "AI")
  UEnvQuery *SpawnPowerupQuery;

  UPROPERTY(EditDefaultsOnly, Category = "AI")
  UCurveFloat *DifficultyCurve;

  FTimerHandle TimerHandle_SpawnBots;

  UPROPERTY(EditDefaultsOnly, Category = "AI") float SpawnBotTimerInterval;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
  int32 CreditsPerKill;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
  int32 MaxPowerupCount;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
  int32 MinPowerupDistance;

  UFUNCTION()
  void SpawnBotTimerElapsed();

  UFUNCTION()
  void
  OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper *QueryInstance,
                           EEnvQueryStatus::Type QueryStatus);
  UFUNCTION()
  void
  OnSpawnPowerupQueryCompleted(UEnvQueryInstanceBlueprintWrapper *QueryInstance,
                               EEnvQueryStatus::Type QueryStatus);

public:
  ASGameModeBase();

  void InitGame(const FString &MapName, const FString &Options,
                FString &ErrorMessage) override;

  virtual void StartPlay() override;

  void
  HandleStartingNewPlayer_Implementation(APlayerController *NewPlayer) override;

  UFUNCTION(Exec)
  void KillAll();

  UFUNCTION(BlueprintCallable)
  virtual void OnActorKilled(AActor *VictimActor, AActor *Killer);

  UFUNCTION()
  void RespawnPlayerElapsed(AController *Controller);

  UFUNCTION(BlueprintCallable, Category = "SaveGame")
  void WriteSaveGame();

  void LoadSaveGame();
};
