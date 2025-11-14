// Fill out your copyright notice in the Description page of Project Settings.
#include "Items/Weapons/Weapon.h"
#include "Character/SlashCharacter.h"

void AWeapon::EquippedTo(USceneComponent* AttachedTo, const FName& Socket)
{
	if (AttachedTo)
	{
		AttachWeaponToSocket(AttachedTo, Socket);
		ItemState = EItemState::EIS_Equipped;
	}
}

void AWeapon::AttachWeaponToSocket(USceneComponent* AttachedTo, const FName& Socket)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(AttachedTo, TransformRules, Socket);
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
