#pragma once

#include "Enums\InventoryEnum\ItemEnum.h"
#include "Str_ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
   GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FString ItemName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FString Description;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bIsStackable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 Amount;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    class UTexture2D* IconImage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    EInventoryItem InventoryItem;
};
