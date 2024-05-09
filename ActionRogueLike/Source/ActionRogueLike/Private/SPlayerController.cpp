// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerController.h"
#include "Blueprint/UserWidget.h"

void ASPlayerController::SetPawn(APawn *InPawn) {
  Super::SetPawn(InPawn);
  OnPawnChanged.Broadcast(InPawn);
}

void ASPlayerController::OnRep_PlayerState() {
  Super::OnRep_PlayerState();
  OnPlayerStateReceived.Broadcast(PlayerState);
}

void ASPlayerController::SetupInputComponent() {
  Super::SetupInputComponent();

  InputComponent->BindAction("PauseMenu", EInputEvent::IE_Pressed, this,
                             &ASPlayerController::TogglePauseMenu);
}

void ASPlayerController::TogglePauseMenu() {
  if (PauseMenuWidgetInstance && PauseMenuWidgetInstance->IsInViewport()) {
    PauseMenuWidgetInstance->RemoveFromParent();
    PauseMenuWidgetInstance = nullptr;

    bShowMouseCursor = false;
    SetInputMode(FInputModeGameOnly());
    return;
  }

  PauseMenuWidgetInstance =
      CreateWidget<UUserWidget>(this, PauseMenuWidgetClass);
  if (PauseMenuWidgetInstance) {
    PauseMenuWidgetInstance->AddToViewport(100);

    bShowMouseCursor = true;
    SetInputMode(FInputModeUIOnly());
  }
}
