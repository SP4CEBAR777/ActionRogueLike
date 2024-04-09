// Fill out your copyright notice in the Description page of Project Settings.

#include "STargetDummy.h"
#include "Components/StaticMeshComponent.h"
#include "SAttributeComponent.h"

// Sets default values
ASTargetDummy::ASTargetDummy() {
  MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
  RootComponent = MeshComp;
  AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
  AttributeComp->OnHealthChanged.AddDynamic(this,
                                            &ASTargetDummy::OnHealthChanged);
}

// Called when the game starts or when spawned
void ASTargetDummy::BeginPlay() { Super::BeginPlay(); }

void ASTargetDummy::OnHealthChanged(AActor *InstigatorActor,
                                    USAttributeComponent *OwningComp,
                                    float NewHealth, float Delta) {
  if (Delta < 0.0f) {
    MeshComp->SetScalarParameterValueOnMaterials("TimeToHit",
                                                 GetWorld()->TimeSeconds);
  }
}
