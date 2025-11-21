// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"


class UBoxComponent;
class USoundBase;

UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	void EquippedTo(USceneComponent* AttachedTo, const FName& Socket);
	void AttachWeaponToSocket(USceneComponent* AttachedTo, const FName& Socket);

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
	FORCEINLINE void ClearActorsToIgnore() { ActorsToIgnore.Empty(); }

protected:
	virtual void BeginPlay() override;

	// Overrides should not use UFUNCTION macro, it is inherited from parent class
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult
	) override;

	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* EquipSound = nullptr; //Unique to different weapon, set in blueprint

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd = nullptr;

	TArray<AActor*> ActorsToIgnore; //cleared every time after swing collision box is off
};
