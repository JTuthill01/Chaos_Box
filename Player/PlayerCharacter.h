#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include <Interfaces/Player/PlayerInterface.h>
#include <Structs/PlayerStats/Str_PlayerStats.h>
#include <Structs/ItemData/Str_ItemData.h>
#include <Enums/WeaponEnums/WeaponEnums.h>
#include "PlayerCharacter.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class APlayerCharacter : public ACharacter, public IPlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	FORCEINLINE class UCameraComponent* GetPlayerCamera() { return Camera; }
	FORCEINLINE class AWeaponBase* GetCurrentWeapon() { return CurrentWeapon; }
	FORCEINLINE USkeletalMeshComponent* GetPlayerArms() { return Arms; }
	FORCEINLINE TArray<FItemData> GetInventoryData() { return InventoryData; }
	FORCEINLINE void SetWeaponIndex(int32 NewIndex) { CurrentWeaponIndex = NewIndex; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual APlayerCharacter* GetPlayerRef_Implementation() override;

public:
	void FirePressed();
	void FireReleased();
	void Interact();
	void Reload();

#pragma endregion

private:
	void InteractableScanner();

	void SpawnWeapon();

	bool AddItem(FHitResult TraceResult);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SK_Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Arms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AWeaponBase> CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AWeaponBase> WeaponToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montages, meta = (AllowPrivateAccess = "true"))
	TArray< TObjectPtr<class UAnimMontage> > WeaponFireMontage;

private:
	UPROPERTY()
	TObjectPtr<UAnimInstance> PlayerAnimInstance;

private:
	bool bIsInWidget;

	float ScanTimer;

	int32 CurrentWeaponIndex;

	EHasWeapon HasWeapon;

	FTimerHandle ScanTimerHandle;

	TArray<FItemData> InventoryData;
};
