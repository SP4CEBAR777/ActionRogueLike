// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor,
                                        public ISGameplayInterface {
  GENERATED_BODY()

public:
  ASItemChest();

  UPROPERTY(EditDefaultsOnly)
  float TargetPitch;

  void Interact_Implementation(APawn *InstigatorPawn);

  void OnActorLoaded_Implementation();

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadonly)
  UStaticMeshComponent *LidMesh;

  UPROPERTY(VisibleAnywhere)
  UStaticMeshComponent *BaseMesh;

  UPROPERTY(ReplicatedUsing = "OnRep_LidOpened", BlueprintReadonly,
            SaveGame) // RepNotify
  bool bLidOpened;

  UFUNCTION()
  void OnRep_LidOpened();
};
