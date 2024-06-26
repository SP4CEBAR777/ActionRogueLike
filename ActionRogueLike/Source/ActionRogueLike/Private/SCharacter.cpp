// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"

ASCharacter::ASCharacter() {
  PrimaryActorTick.bCanEverTick = true;

  SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
  SpringArmComp->bUsePawnControlRotation = true;
  SpringArmComp->SetupAttachment(RootComponent);
  SpringArmComp->SetUsingAbsoluteRotation(true);

  CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
  CameraComp->SetupAttachment(SpringArmComp);

  InteractionComp =
      CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

  AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

  ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

  // Mesh reacts to incoming projectiles, capusule component don't
  GetMesh()->SetGenerateOverlapEvents(true);
  GetCapsuleComponent()->SetGenerateOverlapEvents(false);

  GetCharacterMovement()->bOrientRotationToMovement = true;
  bUseControllerRotationYaw = false;

  TimeToHitParamName = "TimeToHit";

  RageFraction = -0.1f;
}

void ASCharacter::PostInitializeComponents() {
  Super::PostInitializeComponents();

  AttributeComp->OnHealthChanged.AddDynamic(this,
                                            &ASCharacter::OnHealthChanged);
}

FVector ASCharacter::GetPawnViewLocation() const {
  return CameraComp->GetComponentLocation();
}

void ASCharacter::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  PlayerInputComponent->BindAxis("MoveForward", this,
                                 &ASCharacter::MoveForward);
  PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

  PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
  PlayerInputComponent->BindAxis("LookUp", this,
                                 &APawn::AddControllerPitchInput);

  PlayerInputComponent->BindAction("Sprint", IE_Pressed, this,
                                   &ASCharacter::SprintStart);

  PlayerInputComponent->BindAction("Sprint", IE_Released, this,
                                   &ASCharacter::SprintStop);

  PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this,
                                   &ASCharacter::PrimaryAttack);
  PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this,
                                   &ASCharacter::BlackHoleAttack);
  PlayerInputComponent->BindAction("Dash", IE_Pressed, this,
                                   &ASCharacter::Dash);
  PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this,
                                   &ASCharacter::PrimaryInteract);

  PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
}

void ASCharacter::MoveForward(float Value) {
  FRotator ControlRot = GetControlRotation();
  ControlRot.Pitch = 0.0f;
  ControlRot.Roll = 0.0f;

  AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value) {
  FRotator ControlRot = GetControlRotation();
  ControlRot.Pitch = 0.0f;
  ControlRot.Roll = 0.0f;

  FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

  AddMovementInput(RightVector, Value);
}

void ASCharacter::SprintStart() {
  ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop() { ActionComp->StopActionByName(this, "Sprint"); }

void ASCharacter::PrimaryAttack() {
  ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::BlackHoleAttack() {
  ActionComp->StartActionByName(this, "BlackHoleAttack");
}

void ASCharacter::Dash() { ActionComp->StartActionByName(this, "Dash"); }

void ASCharacter::PrimaryInteract() {
  if (InteractionComp) {
    InteractionComp->PrimaryInteract();
  }
}

void ASCharacter::OnHealthChanged(AActor *InstigatorActor,
                                  USAttributeComponent *OwningComp,
                                  float NewHealth, float Delta) {
  if (Delta < 0.0f) {
    GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName,
                                                  GetWorld()->TimeSeconds);

    OwningComp->ApplyRageChange(InstigatorActor, Delta * RageFraction);
  }

  if (NewHealth <= 0.0f && Delta < 0.0f) {
    APlayerController *PC = Cast<APlayerController>(GetController());
    DisableInput(PC);

    SetLifeSpan(5.0f);
  }
}

void ASCharacter::HealSelf(float Amount /*= 100*/) {
  AttributeComp->ApplyHealthChange(this, Amount);
}
