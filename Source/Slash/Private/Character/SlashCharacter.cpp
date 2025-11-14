// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SlashCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GroomComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//Disable controller-lead rotation/ rotation should only affect the camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; //Character faces the direction of movement input
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 290.0f, 0.0f);

	//Set up View Camera and SpringArm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.0f;
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	// Set up hair and eyebrow components
	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");
	Eyebrow = CreateDefaultSubobject<UGroomComponent>(TEXT("EyeBrow"));
	Eyebrow->SetupAttachment(GetMesh());
	Eyebrow->AttachmentName = FString("head");

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashMappingContext, 0);
		}
	}
}

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	const FVector2D Direction = Value.Get<FVector2D>();
	if (GetController())
	{
		const FRotator rotation = GetControlRotation();
		// where the character is facing now
		const FRotator YawRotation(0, rotation.Yaw, 0); 
		FRotationMatrix RotationMatrix = FRotationMatrix(YawRotation);
		const FVector Front = RotationMatrix.GetUnitAxis(EAxis::X);
		AddMovementInput(Front, Direction.X);
		const FVector Right = RotationMatrix.GetUnitAxis(EAxis::Y);
		AddMovementInput(Right, Direction.Y);
	}
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Direction = Value.Get<FVector2D>();

	if (GetController())
	{
		AddControllerYawInput(Direction.X);
		AddControllerPitchInput(Direction.Y);
	}
}

void ASlashCharacter::Equip()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon )
	{
		OverlappingWeapon->EquippedTo(GetMesh(), RightHandSocketName);
		EquippedWeapon = OverlappingWeapon;
		OverlappingItem = nullptr; // make sure the item is cleared, otherwise it will try to equip the weapon again  when E is repeatedly pressed 
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	}
	else
	{
		if (CanDisarm())
		{
			PlayArmOrDisArmMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_InteractWithWeapon;
		}
		else if (CanArm())
		{
			PlayArmOrDisArmMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_InteractWithWeapon;
		}
	}
}

void ASlashCharacter::Attack()
{
	if (CanAttack()) {
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking; 
	}
}

bool ASlashCharacter::CanAttack()
{
	if (ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped)
	{
		return true;
	}
	return false;
}

bool ASlashCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped ;
}

bool ASlashCharacter::CanArm()
{
	return (ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon != nullptr);
}

void ASlashCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = "Attack1";
			AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
			break;

		case 1:
			SectionName = "Attack2";
			AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
			break;
		default:
			break;
		}
	}
}

void ASlashCharacter::PlayArmOrDisArmMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::DisarmWeaponToBack()
{
	if (!EquippedWeapon)  return;
	EquippedWeapon->AttachWeaponToSocket(GetMesh(), "SpineSocket");
}

void ASlashCharacter::ArmWeaponToHand()
{
	if (!EquippedWeapon) return;
	EquippedWeapon->AttachWeaponToSocket(GetMesh(), RightHandSocketName);
}

void ASlashCharacter::EquippingFinishing()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		// ETriggerEvent::Started -> one jump per press
		EnhancedComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASlashCharacter::Jump);
		EnhancedComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &ASlashCharacter::Equip);
		EnhancedComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASlashCharacter::Attack);
	}
}

