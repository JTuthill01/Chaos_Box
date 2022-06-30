#include "Player/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Interfaces/Interact/InteractInterface.h>
#include <Kismet/GameplayStatics.h>
#include <Weapons/WeaponBase/WeaponBase.h>
#include "Widgets/InventoryMain/InventoryMainWidget.h"
#include "Pickups/PickupBase/PickupBase.h"

// Sets default values
APlayerCharacter::APlayerCharacter() : bIsInWidget(false), ScanTimer(0.25F), CurrentWeaponIndex(0), HasWeapon(EHasWeapon::EHW_NoWeapon)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->bUsePawnControlRotation = true;

	Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	Arms->SetupAttachment(Camera);
	Arms->SetCastShadow(false);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(ScanTimerHandle, this, &APlayerCharacter::InteractableScanner, ScanTimer, true);

	SpawnWeapon();

	PlayerAnimInstance = Arms->GetAnimInstance();
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearAllTimersForObject(this);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacter::FirePressed()
{
	if (IsValid(CurrentWeapon))
	{
		switch (HasWeapon)
		{
		case EHasWeapon::EHW_NoWeapon:
			break;

		case EHasWeapon::EHW_HasWeapon:

			if (IsValid(CurrentWeapon) && IsValid(PlayerAnimInstance))
			{
				if (CurrentWeapon->MagHasAmmo())
				{
					if (CurrentWeapon->GetCanFire() && WeaponFireMontage.IsValidIndex(CurrentWeaponIndex))
					{
						CurrentWeapon->WeaponFire();

						PlayerAnimInstance->Montage_Play(WeaponFireMontage[CurrentWeaponIndex]);
					}
				}
			}

			break;

		default:
			break;
		}
	}
}

void APlayerCharacter::FireReleased()
{
}

void APlayerCharacter::Reload()
{
	if (IsValid(CurrentWeapon) && IsValid(PlayerAnimInstance))
	{
		if (CurrentWeapon->HasAmmoForReload())
		{
			switch (CurrentWeapon->GetFireType())
			{
			case EWeaponFireType::EWFT_None:
				break;

			case EWeaponFireType::EWFT_Hitscan:
				break;

			case EWeaponFireType::EWFT_Projectile:
				break;

			case EWeaponFireType::EWFT_SpreadScan:
				break;

			default:
				break;
			}
		}
	}
}

void APlayerCharacter::Interact()
{
	FHitResult HitResult;

	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetComponentRotation().Vector() * 400.F);

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(CurrentWeapon);
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	const bool bCanInteract = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, TraceObjects, true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (bCanInteract)
	{
		if (HitResult.GetActor())
		{
			if (HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
			{
				if (HitResult.GetActor()->GetClass()->IsChildOf(APickupBase::StaticClass()))
				{
					const bool bIsSuccessful = AddItem(HitResult);

					if (bIsSuccessful)
						IInteractInterface::Execute_InteractWithObject(HitResult.GetActor());

					else
						return;
				}

				else 
					IInteractInterface::Execute_InteractWithObject(HitResult.GetActor());
			}
		}
	}
}

void APlayerCharacter::InteractableScanner()
{
	FHitResult HitResult;

	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetComponentRotation().Vector() * 400.F);

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(CurrentWeapon);
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	const bool bIsInteractable = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, TraceObjects, true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (bIsInteractable)
	{
		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
			IInteractInterface::Execute_InteractableFound(HitResult.GetActor());
	}
}

void APlayerCharacter::SpawnWeapon()
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector Location = Arms->GetComponentLocation();
	FRotator Rotation = Arms->GetComponentRotation();

	CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponToSpawn, Location, Rotation, Params);

	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale, CurrentWeapon->GetSocketName());

		HasWeapon = EHasWeapon::EHW_HasWeapon;

		CurrentWeapon->SetWeaponStats(CurrentWeapon->GetCurrentWeaponEnumName());

		CurrentWeaponIndex = CurrentWeapon->GetWeaponIndex();
	}
}

bool APlayerCharacter::AddItem(FHitResult TraceResult)
{
	if (TraceResult.GetActor()->GetClass()->IsChildOf(APickupBase::StaticClass()))
	{
		TObjectPtr<APickupBase> TempPickup = Cast<APickupBase>(TraceResult.GetActor());

		if (IsValid(TempPickup))
		{
			InventoryData.Add(TempPickup->GetPickupData());

			return true;
		}

		else
			return false;
	}

	return false;
}

APlayerCharacter* APlayerCharacter::GetPlayerRef_Implementation() { return this; }
