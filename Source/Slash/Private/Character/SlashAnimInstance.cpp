// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SlashAnimInstance.h"
#include "Character/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());
	if (SlashCharacter)
	{
		//Get the character movement component 
		SlashCharacterMovement = SlashCharacter->GetCharacterMovement();
	}
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (SlashCharacterMovement)
	{
		//GroundSpeed = the magnitude of the velocity vector in the X and Y plane
		GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCharacterMovement->Velocity); 
		IsFalling = SlashCharacterMovement->IsFalling();
		CharacterState = SlashCharacter->GetCharacterState();

		UE_LOG(LogTemp, Warning, TEXT("Character State: %s"), *UEnum::GetDisplayValueAsText(CharacterState).ToString());

	}
}
