#include "Widgets/InventoryMain/InventoryMainWidget.h"
#include "Player/PlayerCharacter.h"
#include "Interfaces/Player/PlayerInterface.h"
#include "Components/ScrollBox.h"
#include "Widgets/ItemSlot/ItemSlot.h"
#include "Controllers/PlayerController/PlayerCharacterController.h"
#include <Kismet/GameplayStatics.h>

UInventoryMainWidget::UInventoryMainWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UInventoryMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = true;

	PlayerRef = IPlayerInterface::Execute_GetPlayerRef(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	BuildInventory();

	BuildGroundItems();
}

void UInventoryMainWidget::BuildInventory()
{
	if (IsValid(PlayerRef))
	{
		ItemScrollbox->ClearChildren();
	
		APlayerCharacterController* PC = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			
		if (IsValid(PC))
		{
			for (int32 i = 0; i < PlayerRef->GetInventoryData().Num(); ++i)
			{
				TObjectPtr<UItemSlot> ISlot = CreateWidget<UItemSlot>(PC, ItemSlotSub);
	
				if (PlayerRef->GetInventoryData().IsValidIndex(i))
				{
					ISlot->ItemData = PlayerRef->GetInventoryData()[i];
	
					ItemScrollbox->AddChild(ISlot);
				}
			}
		}
	}
}

void UInventoryMainWidget::BuildGroundItems()
{
	if (IsValid(PlayerRef))
	{
		GroundItems->ClearChildren();
	
		APlayerCharacterController* PC = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
		if (IsValid(PC))
		{
			for (int32 i = 0; i < PlayerRef->GetInventoryData().Num(); ++i)
			{
				TObjectPtr<UItemSlot> ISlot = CreateWidget<UItemSlot>(PC, GroundSlotSub);
	
				if (PlayerRef->GetInventoryData().IsValidIndex(i))
				{
					ISlot->ItemData = PlayerRef->GetInventoryData()[i];
	
					GroundItems->AddChild(ISlot);
				}
			}
		}
	}
}
