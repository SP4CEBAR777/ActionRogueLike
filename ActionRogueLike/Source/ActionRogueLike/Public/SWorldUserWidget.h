// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "SWorldUserWidget.generated.h"

class USizeBox;

/**
 *
 */
UCLASS() class ACTIONROGUELIKE_API USWorldUserWidget : public UUserWidget {
  GENERATED_BODY()

protected:
  UPROPERTY(meta = (BindWidget))
  USizeBox *ParentSizeBox;

  virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime);

public:
  UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn = true))
  AActor *AttachedActor;

  UPROPERTY(EditAnywhere, Category = "UI")
  FVector WorldOffset;
};
