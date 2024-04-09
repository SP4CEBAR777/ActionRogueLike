// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"

// Sets default values
ASCharacter::ASCharacter() {
  SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
  SpringArmComp->SetupAttachment(RootComponent);

  CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
  CameraComp->SetupAttachment(SpringArmComp);

  InteractionComp =
      CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

  AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

  AttackAnimDelay = 0.18f;

  HandSocketName = "Muzzle_01";
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void ASCharacter::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  PlayerInputComponent->BindAxis("MoveForward", this,
                                 &ASCharacter::MoveForward);
  PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

  PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
  PlayerInputComponent->BindAxis("LookUp", this,
                                 &APawn::AddControllerPitchInput);

  PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this,
                                   &ASCharacter::PrimaryAttack);
  PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this,
                                   &ASCharacter::BlackHoleAttack);
  PlayerInputComponent->BindAction("Dash", IE_Pressed, this,
                                   &ASCharacter::Dash);
  PlayerInputComponent->BindAction("Jump", IE_Pressed, this,
                                   &ASCharacter::Jump);
  PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this,
                                   &ASCharacter::PrimaryInteract);

  AttributeComp->OnHealthChanged.AddDynamic(this,
                                            &ASCharacter::OnHealthChanged);
}

void ASCharacter::MoveForward(float value) {
  FRotator ControlRot = GetControlRotation();
  ControlRot.Pitch = 0.0f;
  ControlRot.Roll = 0.0f;
  AddMovementInput(ControlRot.Vector(), value);
}

void ASCharacter::MoveRight(float value) {
  FRotator ControlRot = GetControlRotation();
  ControlRot.Pitch = 0.0f;
  ControlRot.Roll = 0.0f;

  FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
  AddMovementInput(RightVector, value);
}

void ASCharacter::StartAttackEffect() {
  PlayAnimMontage(AttackAnim);
  UGameplayStatics::SpawnEmitterAttached(
      CastingEffect, GetMesh(), HandSocketName, FVector::ZeroVector,
      FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
}

void ASCharacter::PrimaryAttack() {
  StartAttackEffect();
  GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this,
                                  &ASCharacter::PrimaryAttack_TimeElapsed,
                                  AttackAnimDelay);
}

void ASCharacter::PrimaryAttack_TimeElapsed() {
  SpawnProjectile(ProjectileClass);
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn) {
  if (ensure(ClassToSpawn)) {
    FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Instigator = this;

    FHitResult Hit;
    FVector TraceStart = CameraComp->GetComponentLocation();
    FVector TraceEnd = CameraComp->GetComponentLocation() +
                       (GetControlRotation().Vector() * 5000);

    FCollisionShape Shape;
    Shape.SetSphere(20.0f);

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    FCollisionObjectQueryParams ObjParams;
    ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
    ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
    ObjParams.AddObjectTypesToQuery(ECC_Pawn);

    FRotator ProjRotation;
    if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd,
                                            FQuat::Identity, ObjParams, Shape,
                                            Params)) {
      ProjRotation =
          FRotationMatrix::MakeFromX(Hit.ImpactPoint - HandLocation).Rotator();
    } else {
      ProjRotation =
          FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
    }

    FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
    GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
  }
}

void ASCharacter::BlackHoleAttack() {
  StartAttackEffect();
  GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this,
                                  &ASCharacter::BlackHoleAttack_TimeElapsed,
                                  AttackAnimDelay);
}

void ASCharacter::BlackHoleAttack_TimeElapsed() {
  SpawnProjectile(BlackHoleProjectileCalss);
}

void ASCharacter::Dash() {
  StartAttackEffect();
  GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this,
                                  &ASCharacter::Dash_TimeElapsed,
                                  AttackAnimDelay);
}

void ASCharacter::Dash_TimeElapsed() { SpawnProjectile(DashProjectileClass); }

void ASCharacter::PrimaryInteract() { InteractionComp->PrimaryInteract(); }

void ASCharacter::OnHealthChanged(AActor *InstigatorActor,
                                  USAttributeComponent *OwningComp,
                                  float NewHealth, float Delta) {

  if (Delta < 0.0f) {
    GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit",
                                                  GetWorld()->TimeSeconds);
  }

  if (NewHealth <= 0.0f && Delta < 0.0f) {

    APlayerController *PC = Cast<APlayerController>(GetController());
    DisableInput(PC);
  }
}
