#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interact/InteractInterface.h"
#include "Structs/ItemData/Str_ItemData.h"
#include "PickupBase.generated.h"

UCLASS()
class APickupBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBase();

public:
	FORCEINLINE FItemData GetPickupData() { return PickupData; }

public:
	void InteractableFound_Implementation() override;

	void InteractWithObject_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PickupRoot)
	TObjectPtr<USceneComponent> PickupRootComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PickupMesh)
	TObjectPtr<UStaticMeshComponent> PickupMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PickupSphere)
	TObjectPtr<class UBoxComponent> PickupBox;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BoxData, meta = (AllowPrivateAccess = "true"))
	FVector BoxSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PickupData, meta = (AllowPrivateAccess = "true"))
	FItemData PickupData;
};
