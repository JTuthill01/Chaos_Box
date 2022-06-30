#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMainWidget.generated.h"

UCLASS()
class UInventoryMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInventoryMainWidget(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION()
	void BuildInventory();

	UFUNCTION()
	void BuildGroundItems();

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> ItemScrollbox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> GroundItems;

private:
	UPROPERTY()
	TObjectPtr<class APlayerCharacter> PlayerRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemSlotSubclass, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> ItemSlotSub;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemSlotSubclass, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> GroundSlotSub;
};
