#include "SProjectileBase.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

ASProjectileBase::ASProjectileBase() {
  SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
  SphereComp->SetCollisionProfileName("Projectile");

  RootComponent = SphereComp;

  EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
  EffectComp->SetupAttachment(RootComponent);

  AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
  AudioComp->SetupAttachment(RootComponent);

  MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(
      "ProjectileMoveComp");
  MoveComp->bRotationFollowsVelocity = true;
  MoveComp->bInitialVelocityInLocalSpace = true;
  MoveComp->ProjectileGravityScale = 0.0f;
  MoveComp->InitialSpeed = 8000.0f;

  bReplicates = true;
}

void ASProjectileBase::OnActorHit(UPrimitiveComponent *HitComponent,
                                  AActor *OtherActor,
                                  UPrimitiveComponent *OtherComp,
                                  FVector NormalImpulse,
                                  const FHitResult &Hit) {
  Explode();
}

// _Implementation from it being marked as BlueprintNativeEvent
void ASProjectileBase::Explode_Implementation() {
  // Check to make sure we aren't already being 'destroyed'
  // Adding ensure to see if we encounter this situation at all
  if (ensure(IsValid(this))) {
    UGameplayStatics::SpawnEmitterAtLocation(
        this, ImpactVFX, GetActorLocation(), GetActorRotation());

    UGameplayStatics::PlaySoundAtLocation(this, ImpactSound,
                                          GetActorLocation());

    Destroy();
  }
}

void ASProjectileBase::PostInitializeComponents() {
  Super::PostInitializeComponents();

  SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);
}
