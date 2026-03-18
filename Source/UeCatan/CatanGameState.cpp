#include "CatanGameState.h"
#include "Net/UnrealNetwork.h"

ACatanGameState::ACatanGameState()
{
	bReplicates = true;
}

void ACatanGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACatanGameState, BoardTiles);
	DOREPLIFETIME(ACatanGameState, CurrentTurnPlayerId);
	DOREPLIFETIME(ACatanGameState, LastDiceRoll);
	DOREPLIFETIME(ACatanGameState, WinningPlayerId);
	DOREPLIFETIME(ACatanGameState, BoardRevision);
	DOREPLIFETIME(ACatanGameState, MatchLog);
}

void ACatanGameState::AddLogMessage(const FString& Message)
{
	MatchLog.Add(Message);
	const int32 MaxLogLines = 10;
	while (MatchLog.Num() > MaxLogLines)
	{
		MatchLog.RemoveAt(0);
	}
}

const FCatanTileState* ACatanGameState::FindTile(int32 TileId) const
{
	return BoardTiles.FindByPredicate([TileId](const FCatanTileState& Tile)
	{
		return Tile.TileId == TileId;
	});
}
