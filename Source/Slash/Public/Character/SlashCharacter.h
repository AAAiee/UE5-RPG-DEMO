// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterState.h"
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
	FORCEINLINE const AItem* GetOverlappingItem() const { return OverlappingItem; }
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

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
	UInputAction* AttackAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* SlashMappingContext = nullptr;

private:

	/*Callbacks for movement*/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Equip();
	void Attack();
	bool CanAttack();

	/*Play Montage*/
	void PlayMontage();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	

private:
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	const FName RightHandSocketName = TEXT("RightHandSocket");

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EActionState ActionState = EActionState::EAS_Unoccupied;

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

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	class UAnimMontage* AttackMontage = nullptr; // initialized in the blueprint
};
