#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Structs/ItemData/Str_ItemData.h>
#include "ItemSlot.generated.h"

UCLASS()
class UItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UItemSlot(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void AddToInventory();

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> PickupText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> GroundButton;

public:
	UPROPERTY()
	FItemData ItemData;

private:
	UPROPERTY()
	TObjectPtr<class APlayerCharacter> PlayerRef;
};
