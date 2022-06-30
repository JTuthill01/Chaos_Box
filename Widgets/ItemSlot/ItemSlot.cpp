#include "Widgets/ItemSlot/ItemSlot.h"
#include "Components/TextBlock.h"
#include "Player/PlayerCharacter.h"
#include "Components/Button.h"
#include "Interfaces/Player/PlayerInterface.h"
#include <Kismet/GameplayStatics.h>

UItemSlot::UItemSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UItemSlot::AddToInventory()
{
}

void UItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	GroundButton->OnClicked.AddDynamic(this, &UItemSlot::AddToInventory);

	PlayerRef = IPlayerInterface::Execute_GetPlayerRef(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	PickupText->SetText(FText::FromString(ItemData.ItemName));
}
