// Fill out your copyright notice in the Description page of Project Settings.

#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel() {
  // Set this actor to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
  RootComponent = MeshComp;
  ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
  ForceComp->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay() {
  Super::BeginPlay();
  MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent *HitComp,
                                   AActor *OtherActor,
                                   UPrimitiveComponent *OtherComp,
                                   FVector NormalImpulse,
                                   const FHitResult &Hit) {
  ForceComp->FireImpulse();
}
