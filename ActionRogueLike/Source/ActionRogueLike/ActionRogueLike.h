// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

static void LogOnScreen(UObject *WorldContext, FString Msg,
                        FColor Color = FColor::White, float Duration = 5.0f) {
  if (!ensure(WorldContext)) {
    return;
  }

  UWorld *World = WorldContext->GetWorld();
  if (!ensure(World)) {
    return;
  }

  FString NetPrefix = World->IsNetMode(NM_Client) ? "[CLIENT] " : "[SERVER] ";
  if (GEngine) {
    GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
  }
}

DECLARE_STATS_GROUP(TEXT("STANDARD_Game"), STATGROUP_STANDARD,
                    STATCAT_Advanced);
