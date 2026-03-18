#include "CatanTypes.h"

void FCatanResourceSet::Add(ECatanResource Resource, int32 Amount)
{
	switch (Resource)
	{
	case ECatanResource::Wood:
		Wood += Amount;
		break;
	case ECatanResource::Brick:
		Brick += Amount;
		break;
	case ECatanResource::Wheat:
		Wheat += Amount;
		break;
	case ECatanResource::Sheep:
		Sheep += Amount;
		break;
	case ECatanResource::Ore:
		Ore += Amount;
		break;
	default:
		break;
	}
}

bool FCatanResourceSet::HasAtLeast(const FCatanResourceSet& Cost) const
{
	return Wood >= Cost.Wood
		&& Brick >= Cost.Brick
		&& Wheat >= Cost.Wheat
		&& Sheep >= Cost.Sheep
		&& Ore >= Cost.Ore;
}

void FCatanResourceSet::Spend(const FCatanResourceSet& Cost)
{
	Wood -= Cost.Wood;
	Brick -= Cost.Brick;
	Wheat -= Cost.Wheat;
	Sheep -= Cost.Sheep;
	Ore -= Cost.Ore;
}

FString FCatanResourceSet::ToDisplayString() const
{
	return FString::Printf(TEXT("Wood:%d  Brick:%d  Wheat:%d  Sheep:%d  Ore:%d"), Wood, Brick, Wheat, Sheep, Ore);
}

FCatanResourceSet FCatanResourceSet::SettlementCost()
{
	FCatanResourceSet Cost;
	Cost.Wood = 1;
	Cost.Brick = 1;
	Cost.Wheat = 1;
	Cost.Sheep = 1;
	return Cost;
}

FString CatanStatics::ResourceToString(ECatanResource Resource)
{
	switch (Resource)
	{
	case ECatanResource::Wood:
		return TEXT("Wood");
	case ECatanResource::Brick:
		return TEXT("Brick");
	case ECatanResource::Wheat:
		return TEXT("Wheat");
	case ECatanResource::Sheep:
		return TEXT("Sheep");
	case ECatanResource::Ore:
		return TEXT("Ore");
	default:
		return TEXT("Desert");
	}
}

FColor CatanStatics::ResourceToColor(ECatanResource Resource)
{
	switch (Resource)
	{
	case ECatanResource::Wood:
		return FColor(46, 125, 50);
	case ECatanResource::Brick:
		return FColor(183, 28, 28);
	case ECatanResource::Wheat:
		return FColor(251, 192, 45);
	case ECatanResource::Sheep:
		return FColor(102, 187, 106);
	case ECatanResource::Ore:
		return FColor(120, 144, 156);
	default:
		return FColor::Black;
	}
}
