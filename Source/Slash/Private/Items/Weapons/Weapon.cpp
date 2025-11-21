// Fill out your copyright notice in the Description page of Project Settings.
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h" // sound play
#include "Kismet/KismetSystemLibrary.h" // box trace
#include "Character/SlashCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Interfaces/HitInterface.h"


AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());

	//Overlap with all types except Pawns
	//start off no collision
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	//Box trace start and end, using two invisible scene components as marks to form a line 
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::EquippedTo(USceneComponent* AttachedTo, const FName& Socket)
{
	AttachWeaponToSocket(AttachedTo, Socket);
	ItemState = EItemState::EIS_Equipped;

	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,
			EquipSound,
			GetActorLocation());
	}

	//After equip the weapon, the weapon's sphere is disabled to prevent more overlap events are generated-> leads to overlapping item is set to the weapon again
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::AttachWeaponToSocket(USceneComponent* AttachedTo, const FName& Socket)
{
	if (AttachedTo) {
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		ItemMesh->AttachToComponent(AttachedTo, TransformRules, Socket);
	}
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	//register the overlap callback for the box component
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// debug info 
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);


	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter)
	{
		SlashCharacter->SetOverlappingItem(this);
	}

}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//debug info
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter)
	{
		SlashCharacter->SetOverlappingItem(nullptr);
	}

}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();
	
	FHitResult HitResult;
	UKismetSystemLibrary::BoxTraceSingle(this, Start, End, FVector(5.f, 5.f, 5.f), BoxTraceStart->GetComponentRotation(), ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true); 

	if (HitResult.GetActor())
	{
		ActorsToIgnore.AddUnique(HitResult.GetActor());
		IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor());
		if (HitInterface)
		{
			HitInterface->GetHit(HitResult.ImpactPoint);
		}
	}
}
