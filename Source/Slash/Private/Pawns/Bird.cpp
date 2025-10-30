#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h" 
#include "Camera/CameraComponent.h"


ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule")); // usually make this as root
	SetRootComponent(Capsule);  

	//Capsule: the collision shape -> so we set a size to fit the bird
	Capsule->SetCapsuleHalfHeight(20.0f);
	Capsule->SetCapsuleRadius(15.0f);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(GetRootComponent());  //set up the attachment to the root

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("BirdMovement"));
	FloatingPawnMovement->SetUpdatedComponent(GetRootComponent());

	//SpringArm attached to the root
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm")); 
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.0f; //camera distance offset

	// View camera attached to the SpringArm
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	// control this pawn with the first player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ABird::BeginPlay()
{
	Super::BeginPlay();

	// Set BirdMappingContext as the current InputMappingContext for the Subsystem
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BirdMappingContext, 0);
		}
	}
}

void ABird::Move(const FInputActionValue& Value)
{
	// we have a bool type InputAction
	const float Direction = Value.Get<float>();
	if (GetController() && Direction != 0.0f)
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, Direction); // -1 move back, 1 move forward
	}
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Binding callbacks
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent> (PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move); // called when the function is triggered on succeeded
	}
}

