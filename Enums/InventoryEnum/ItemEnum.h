#pragma once

UENUM(BlueprintType)
enum class EInventoryItem : uint8 { EII_None UMETA(DisplayName = "None"), EII_Weapon UMETA(DisplayName = "Weapon"),
EII_Armor UMETA(DisplayName = "Armor"), EII_Health UMETA(DisplayName = "Health"), EII_Ammo UMETA(DisplayName = "Ammo")};