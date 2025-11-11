// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//BluePrintCallable: callable from Blueprint
	//BluePrintPure : do not change any parameter

protected:

	UFUNCTION() //Must be exposed to the blue print, otherwise wouldn't bind
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult
	);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh = nullptr;

private:
	// VisibleDefaultsOnly: Visible in the Blueprint editor, but not editable
		// Notice: This is not an instance so running time will not be updated
	// VisibleInstanceOnly: Visible per instance in its Detail Panel, but not editable
	// VisibleAnywhere: Visible in both Blueprint editor and per instance but not editable
	UPROPERTY(VisibleInstanceOnly); 
	float RunningTime = 0.f;

	// BluePrintRead: Allow getter for the variable in Blueprint's event graph
	// BluePrintReadWrite: Allow getting and setting value in Blueprint's event graph
		//Notice: These two can not be used on a private member, make them protected or public first
	// However, they can work if we add another specifier 
		// meta = (AllowPrivateAccess = "true")
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Oscillation Parameters", meta = (AllowPrivateAccess = "true")) 
	float Amplitude = 0.25f;

	// EditDefaultOnly: Allow editing in the Blueprint editor only
	// EditInstanceOnly: Allow editing per instance in its Detail Panel
	// EditAnywhere: Allow editing in both Blueprint editor and per instance
		// Notice: Change in Instance will override the Blueprint default value
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Oscillation Parameters", meta = (AllowPrivateAccess = "true")) 
	float TimeConstant = 10.0f;

	UPROPERTY(VisibleAnyWhere)
	class USphereComponent* Sphere = nullptr;
};
