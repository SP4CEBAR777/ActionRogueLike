// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameModeBase.h"
#include "AI/SAICharacter.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"

static TAutoConsoleVariable<bool>
    CVarSpawnBots(TEXT("su.SpawnBots"), false,
                  TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase() {
  SpawnBotTimerInterval = 2.0f;
  CreditsPerKill = 20;

  MaxPowerupCount = 10;
  MinPowerupDistance = 2000;

  PlayerStateClass = ASPlayerState::StaticClass();
}

void ASGameModeBase::StartPlay() {
  Super::StartPlay();

  GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this,
                                  &ASGameModeBase::SpawnBotTimerElapsed,
                                  SpawnBotTimerInterval, true);

  if (ensure(PowerupClasses.Num() > 0)) {
    UEnvQueryInstanceBlueprintWrapper *QueryInstance =
        UEnvQueryManager::RunEQSQuery(this, SpawnPowerupQuery, this,
                                      EEnvQueryRunMode::AllMatching, nullptr);
    if (ensure(QueryInstance)) {
      QueryInstance->GetOnQueryFinishedEvent().AddDynamic(
          this, &ASGameModeBase::OnSpawnPowerupQueryCompleted);
    }
  }
}

void ASGameModeBase::SpawnBotTimerElapsed() {
  if (!CVarSpawnBots.GetValueOnGameThread()) {
    UE_LOG(LogTemp, Warning,
           TEXT(" Bot spawning disable via cvar 'CVarSpawnBots'."));
    return;
  }

  int32 NumOfAliveBots = 0;
  for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
    ASAICharacter *Bot = *It;

    USAttributeComponent *AttributeComp =
        USAttributeComponent::GetAttributes(Bot);
    if (ensure(AttributeComp) && AttributeComp->IsAlive()) {
      ++NumOfAliveBots;
    }
  }

  UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NumOfAliveBots);

  float MaxBotCount = 10.0f;

  if (DifficultyCurve) {
    MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
  }

  if (NumOfAliveBots >= MaxBotCount) {
    UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."));
    return;
  }

  UEnvQueryInstanceBlueprintWrapper *QueryInstance =
      UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this,
                                    EEnvQueryRunMode::RandomBest5Pct, nullptr);
  if (ensure(QueryInstance)) {
    QueryInstance->GetOnQueryFinishedEvent().AddDynamic(
        this, &ASGameModeBase::OnSpawnBotQueryCompleted);
  }
}

void ASGameModeBase::OnSpawnBotQueryCompleted(
    UEnvQueryInstanceBlueprintWrapper *QueryInstance,
    EEnvQueryStatus::Type QueryStatus) {
  if (QueryStatus != EEnvQueryStatus::Success) {
    UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
    return;
  }

  TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

  if (Locations.IsValidIndex(0)) {

    GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0],
                                   FRotator::ZeroRotator);

    // Track all the used spawn locations
    DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false,
                    60.0f);
  }
}

void ASGameModeBase::OnSpawnPowerupQueryCompleted(
    UEnvQueryInstanceBlueprintWrapper *QueryInstance,
    EEnvQueryStatus::Type QueryStatus) {
  if (QueryStatus != EEnvQueryStatus::Success) {
    UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
    return;
  }

  TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

  // Keep used locations to easily check distance between points
  TArray<FVector> UsedLocations;

  int32 SpawnCounter = 0;
  // Break out if we reached the desired count or if we have no more potential
  // positions remaining
  while (SpawnCounter < MaxPowerupCount && Locations.Num() > 0) {
    // Pick a random location from remaining points.
    int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

    FVector PickedLocation = Locations[RandomLocationIndex];
    // Remove to avoid picking again
    Locations.RemoveAt(RandomLocationIndex);

    // Check minimum distance requirement
    bool bValidLocation = true;
    for (FVector OtherLocation : UsedLocations) {
      float DistanceTo = (PickedLocation - OtherLocation).Size();

      if (DistanceTo < MinPowerupDistance) {
        // Show skipped locations due to distance
        // DrawDebugSphere(GetWorld(), PickedLocation, 50.0f, 20, FColor::Red,
        // false, 10.0f);

        // too close, skip to next attempt
        bValidLocation = false;
        break;
      }
    }

    // Failed the distance test
    if (!bValidLocation) {
      continue;
    }

    // Pick a random powerup-class
    int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
    TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];

    GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation,
                                   FRotator::ZeroRotator);

    // Keep for distance checks
    UsedLocations.Add(PickedLocation);
    ++SpawnCounter;
  }
}

void ASGameModeBase::KillAll() {
  for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
    ASAICharacter *Bot = *It;

    USAttributeComponent *AttributeComp =
        USAttributeComponent::GetAttributes(Bot);
    if (ensure(AttributeComp) && AttributeComp->IsAlive()) {
      AttributeComp->Kill(this);
    }
  }
}

void ASGameModeBase::OnActorKilled(AActor *VictimActor, AActor *Killer) {
  UE_LOG(LogTemp, Log, TEXT("OnActorKilled:Victim:%s, Killer: %s"),
         *GetNameSafe(VictimActor), *GetNameSafe(Killer));

  ASCharacter *Player = Cast<ASCharacter>(VictimActor);
  if (Player) {
    FTimerHandle TimerHandle_RespawnDelay;

    FTimerDelegate Delegate;
    Delegate.BindUFunction(this, "RespawnPlayerElapsed",
                           Player->GetController());

    float RespawnDelay = 2.0f;

    GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate,
                                    RespawnDelay, false);
  }

  APawn *KillerP = Cast<APawn>(Killer);
  if (ASPlayerState *PS = KillerP->GetPlayerState<ASPlayerState>()) {
    PS->AddCredits(CreditsPerKill);
  }
}

void ASGameModeBase::RespawnPlayerElapsed(AController *Controller) {
  if (ensure(Controller)) {
    Controller->UnPossess();

    RestartPlayer(Controller);
  }
}
