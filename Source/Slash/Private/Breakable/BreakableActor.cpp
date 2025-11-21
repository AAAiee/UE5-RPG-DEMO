// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
ABreakableActor::ABreakableActor()
{
	//does not need to tick every frame
	PrimaryActorTick.bCanEverTick = false;

	// it is a scene component's child class
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(Cast<USceneComponent>(GeometryCollection));
	GeometryCollection->SetGenerateOverlapEvents(true);
	

	// So the camera is not affected by the flying pieces
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& Vector)
{

}

