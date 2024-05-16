// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameModeBase.h"
#include "../ActionRogueLike.h"
#include "AI/SAICharacter.h"
#include "DrawDebugHelpers.h"
#include "Engine/AssetManager.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SGameplayInterface.h"
#include "SMonsterData.h"
#include "SPlayerState.h"
#include "SSaveGame.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool>
    CVarSpawnBots(TEXT("su.SpawnBots"), true,
                  TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase() {
  SpawnBotTimerInterval = 2.0f;
  CreditsPerKill = 20;

  MaxPowerupCount = 10;
  MinPowerupDistance = 2000;

  PlayerStateClass = ASPlayerState::StaticClass();

  SlotName = "SaveGame01";
}

void ASGameModeBase::InitGame(const FString &MapName, const FString &Options,
                              FString &ErrorMessage) {
  Super::InitGame(MapName, Options, ErrorMessage);

  LoadSaveGame();
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

void ASGameModeBase::HandleStartingNewPlayer_Implementation(
    APlayerController *NewPlayer) {
  ASPlayerState *PS = NewPlayer->GetPlayerState<ASPlayerState>();
  if (PS) {
    PS->LoadPlayerState(CurrentSaveGame);
  }

  Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ASGameModeBase::SpawnBotTimerElapsed() {
  if (!CVarSpawnBots.GetValueOnGameThread()) {
    UE_LOG(LogTemp, Warning,
           TEXT("Bot spawning disable via cvar 'CVarSpawnBots'."));
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
    TArray<FMonsterInfoRow *> Rows;
    if (MonsterTable) {
      MonsterTable->GetAllRows("", Rows);

      // Get random monster
      int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
      FMonsterInfoRow *SelectedRow = Rows[RandomIndex];

      TArray<FName> Bundles;
      FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(
          this, &ASGameModeBase::OnMonsterLoaded, SelectedRow->MonsterId,
          Locations[0]);
      UAssetManager *Manager = UAssetManager::GetIfInitialized();
      if (Manager) {
        // LogOnScreen(this, "Loading monster...", FColor::Green);
        Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
      }
    }
  }
}

void ASGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId,
                                     FVector SpawnLocation) {
  // LogOnScreen(this, "Finished loading.", FColor::Green);
  UAssetManager *Manager = UAssetManager::GetIfInitialized();
  if (Manager) {
    USMonsterData *MonsterData =
        Cast<USMonsterData>(Manager->GetPrimaryAssetObject(LoadedId));
    if (MonsterData) {
      AActor *NewBot = GetWorld()->SpawnActor<AActor>(
          MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
      if (NewBot) {
        // LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s (%s)"),
        //                                   *GetNameSafe(NewBot),
        //                                   *GetNameSafe(MonsterData)));

        // Grant special actions, buffs etc.
        USActionComponent *ActionComp = Cast<USActionComponent>(
            NewBot->GetComponentByClass(USActionComponent::StaticClass()));
        if (ActionComp) {
          for (TSubclassOf<USAction> ActionClass : MonsterData->Actions) {
            ActionComp->AddAction(NewBot, ActionClass);
          }
        }
        // Track all the used spawn locations
        // DrawDebugSphere(GetWorld(), SpawnLocation, 50.0f, 20, FColor::Blue,
        // false, 60.0f);
      }
    }
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

void ASGameModeBase::WriteSaveGame() {
  for (int32 i = 0; i < GameState->PlayerArray.Num(); ++i) {
    ASPlayerState *PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);
    if (PS) {
      PS->SavePlayerState(CurrentSaveGame);
      break;
    }
  }

  CurrentSaveGame->SavedActors.Empty();

  // Iterate through all actors
  for (FActorIterator It(GetWorld()); It; ++It) {
    AActor *Actor = *It;
    if (!Actor->Implements<USGameplayInterface>()) {
      continue;
    }

    FActorSaveData ActorData;
    ActorData.ActorName = Actor->GetName();
    ActorData.Transform = Actor->GetActorTransform();

    // Pass the array to fill with data from Actor
    FMemoryWriter MemWriter(ActorData.ByteData);

    FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
    // Find only variables with UPROPERTY(SaveGame)
    Ar.ArIsSaveGame = true;
    // Converts Actor's SaveGame UPROPERTIES into binary array
    Actor->Serialize(Ar);

    CurrentSaveGame->SavedActors.Add(ActorData);
  }

  if (UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0)) {
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White,
                                     TEXT("Successfully Save to SaveGame."));
  } else {
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
                                     TEXT("Failed to Save to SaveGame."));
  }
}

void ASGameModeBase::LoadSaveGame() {
  if (UGameplayStatics::DoesSaveGameExist(SlotName, 0)) {
    CurrentSaveGame =
        Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    if (CurrentSaveGame == nullptr) {
      UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
      return;
    }

    for (FActorIterator It(GetWorld()); It; ++It) {
      AActor *Actor = *It;
      if (!Actor->Implements<USGameplayInterface>()) {
        continue;
      }

      for (FActorSaveData ActorData : CurrentSaveGame->SavedActors) {
        if (ActorData.ActorName == Actor->GetName()) {
          Actor->SetActorTransform(ActorData.Transform);

          FMemoryReader MemReader(ActorData.ByteData);

          FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
          // Find only variables with UPROPERTY(SaveGame)
          Ar.ArIsSaveGame = true;
          // Converts binary array back into Actor's SaveGame UPROPERTIES
          Actor->Serialize(Ar);
          ISGameplayInterface::Execute_OnActorLoaded(Actor);

          break;
        }
      }
    }

    UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));
  } else {
    CurrentSaveGame = Cast<USSaveGame>(
        UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));

    UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
  }
}
