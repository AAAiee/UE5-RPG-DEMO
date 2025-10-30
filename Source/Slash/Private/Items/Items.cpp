#include "Items/Items.h"
#include "../DebugMacros.h"

// Sets default values
AItems::AItems()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMeshComponent");
	RootComponent = ItemMesh;
}

// Called when the game starts or when spawned
void AItems::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	float DeltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant);
	AddActorWorldOffset(FVector(0.0f, 0.0f, DeltaZ));
}

