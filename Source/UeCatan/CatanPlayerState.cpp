#include "CatanPlayerState.h"
#include "Net/UnrealNetwork.h"

ACatanPlayerState::ACatanPlayerState()
{
	bReplicates = true;
}

void ACatanPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACatanPlayerState, Resources);
	DOREPLIFETIME(ACatanPlayerState, VictoryPoints);
	DOREPLIFETIME(ACatanPlayerState, SettlementsBuilt);
	DOREPLIFETIME(ACatanPlayerState, TurnOrder);
	DOREPLIFETIME(ACatanPlayerState, bHasRolledThisTurn);
}

void ACatanPlayerState::SetTurnOrder(int32 InTurnOrder)
{
	TurnOrder = InTurnOrder;
}

void ACatanPlayerState::AddResource(ECatanResource Resource, int32 Amount)
{
	Resources.Add(Resource, Amount);
}

bool ACatanPlayerState::CanAffordSettlement() const
{
	return Resources.HasAtLeast(FCatanResourceSet::SettlementCost());
}

bool ACatanPlayerState::SpendSettlementCost()
{
	if (!CanAffordSettlement())
	{
		return false;
	}

	Resources.Spend(FCatanResourceSet::SettlementCost());
	return true;
}

void ACatanPlayerState::AwardSettlement()
{
	++SettlementsBuilt;
	++VictoryPoints;
}

void ACatanPlayerState::ResetForNewTurn()
{
	bHasRolledThisTurn = false;
}
