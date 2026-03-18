#pragma once

#include "CoreMinimal.h"
#include "CatanTypes.generated.h"

/**
 * A tiny set of resources inspired by Catan.
 * We keep the enum Blueprint-friendly so the project can be extended in-editor later.
 */
UENUM(BlueprintType)
enum class ECatanResource : uint8
{
	Wood,
	Brick,
	Wheat,
	Sheep,
	Ore,
	Desert
};

/**
 * Compact replicated resource inventory used by every player.
 * Using a struct keeps the data easy to replicate, inspect, and print in debug HUD text.
 */
USTRUCT(BlueprintType)
struct FCatanResourceSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Wood = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Brick = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Wheat = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Sheep = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Ore = 0;

	void Add(ECatanResource Resource, int32 Amount);
	bool HasAtLeast(const FCatanResourceSet& Cost) const;
	void Spend(const FCatanResourceSet& Cost);
	FString ToDisplayString() const;

	static FCatanResourceSet SettlementCost();
};

/**
 * A simplified tile state. In this prototype a settlement claims an entire tile,
 * which dramatically reduces the amount of graph logic while still creating a
 * turn-based multiplayer board game loop.
 */
USTRUCT(BlueprintType)
struct FCatanTileState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TileId = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECatanResource Resource = ECatanResource::Desert;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DiceNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector WorldLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SettlementOwnerPlayerId = INDEX_NONE;
};

namespace CatanStatics
{
	FString ResourceToString(ECatanResource Resource);
	FColor ResourceToColor(ECatanResource Resource);
}
