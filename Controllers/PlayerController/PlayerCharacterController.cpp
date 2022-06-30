#include "Controllers/PlayerController/PlayerCharacterController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "Interfaces/Player/PlayerInterface.h"
#include "Widgets/InventoryMain/InventoryMainWidget.h"

APlayerCharacterController::APlayerCharacterController() = default;

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = IPlayerInterface::Execute_GetPlayerRef(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
		Subsystem->ClearAllMappings();
		// Add each mapping context, along with their priority values. Higher values out prioritize lower values.
		Subsystem->AddMappingContext(BaseMappingContext, BaseMappingPriority);
	}

	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (JumpAction)
		{
			PlayerEnhancedInputComponent->BindAction(WeaponFireAction, ETriggerEvent::Started, this, &APlayerCharacterController::StartJump);
			PlayerEnhancedInputComponent->BindAction(WeaponFireAction, ETriggerEvent::Completed, this, &APlayerCharacterController::EndJump);
		}

		if (WeaponFireAction)
		{
			PlayerEnhancedInputComponent->BindAction(WeaponFireAction, ETriggerEvent::Started, this, &APlayerCharacterController::FirePressed);
			PlayerEnhancedInputComponent->BindAction(WeaponFireAction, ETriggerEvent::Completed, this, &APlayerCharacterController::FireReleased);
		}

		if (InventoryAction)
		{
			PlayerEnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &APlayerCharacterController::InputModeUI);
			PlayerEnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Completed, this, &APlayerCharacterController::InputModeGame);
		}

		if (CrouchAction)
			PlayerEnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacterController::CrouchPressed);

		if (MovementAction)
			PlayerEnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Move);

		if (LookAction)
			PlayerEnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Look);

		if (InteractAction)
			PlayerEnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Interact);

		if (WeaponReloadAction)
			PlayerEnhancedInputComponent->BindAction(WeaponReloadAction, ETriggerEvent::Started, this, &APlayerCharacterController::Reload);
	}
}

void APlayerCharacterController::Move(const FInputActionValue& Value)
{
	if (Value.GetMagnitude() != 0.F)
	{
		GetPawn()->AddMovementInput(GetPawn()->GetActorForwardVector(), Value[1]);
		GetPawn()->AddMovementInput(GetPawn()->GetActorRightVector(), Value[0]);
	}
}

void APlayerCharacterController::Look(const FInputActionValue& Value)
{
	AddPitchInput(Value[1] * -1);
	AddYawInput(Value[0]);
}

void APlayerCharacterController::StartJump()
{
	if (IsValid(PlayerRef))
		PlayerRef->Jump();
}

void APlayerCharacterController::EndJump()
{
	if (IsValid(PlayerRef))
		PlayerRef->StopJumping();
}

void APlayerCharacterController::CrouchPressed()
{
}

void APlayerCharacterController::FirePressed()
{
	if (IsValid(PlayerRef))
		PlayerRef->FirePressed();
}

void APlayerCharacterController::FireReleased()
{
	if (IsValid(PlayerRef))
		PlayerRef->FireReleased();
}

void APlayerCharacterController::Interact()
{
	if (IsValid(PlayerRef))
		PlayerRef->Interact();
}

void APlayerCharacterController::Reload()
{
	if (IsValid(PlayerRef))
		PlayerRef->Reload();
}

void APlayerCharacterController::InputModeUI()
{
	SetShowMouseCursor(true);

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);

	SetInputMode(InputMode);
	SetIgnoreLookInput(true);

	if (!IsValid(InventoryMainWidget))
	{
		InventoryMainWidget = CreateWidget<UInventoryMainWidget>(this, InventoryWidgetSubclass);

		InventoryMainWidget->SetUserFocus(this);

		InventoryMainWidget->AddToViewport();

		InventoryMainWidget->BuildInventory();

		InventoryMainWidget->BuildGroundItems();
	}

	else if (IsValid(InventoryMainWidget))
	{
		InventoryMainWidget->SetVisibility(ESlateVisibility::Visible);

		InventoryMainWidget->BuildInventory();

		InventoryMainWidget->BuildGroundItems();
	}

	else
		return;
}

void APlayerCharacterController::InputModeGame()
{
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(true);

	if (IsValid(InventoryMainWidget))
	{
		SetShowMouseCursor(false);

		SetIgnoreLookInput(false);

		SetInputMode(InputMode);

		if (InventoryMainWidget->IsInViewport())
			InventoryMainWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	else
		return;
}

APlayerCharacterController* APlayerCharacterController::GetPlayerControllerRef_Implementation() { return this; }
