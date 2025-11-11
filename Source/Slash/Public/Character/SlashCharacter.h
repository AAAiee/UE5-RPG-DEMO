// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SlashCharacter.generated.h"

class AItem;

UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	FORCEINLINE const AItem* GetOverlappingItem() { return OverlappingItem; }
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }

protected:
	virtual void BeginPlay() override;

protected:
	// input system components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MoveAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* LookAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* EquipAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* SlashMappingContext = nullptr;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Equip();

private:
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* ViewCamera = nullptr;
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, Category=Hair)
	class UGroomComponent* Hair = nullptr;

	UPROPERTY(VisibleAnywhere, Category=Eyebrow)
	UGroomComponent* Eyebrow = nullptr;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem = nullptr;

	const FName RightHandSocketName = TEXT("RightHandSocket");
};
