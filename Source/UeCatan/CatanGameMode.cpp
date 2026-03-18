#include "CatanGameMode.h"
#include "CatanBoardActor.h"
#include "CatanCameraPawn.h"
#include "CatanGameState.h"
#include "CatanHUD.h"
#include "CatanPlayerController.h"
#include "CatanPlayerState.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"

ACatanGameMode::ACatanGameMode()
{
	GameStateClass = ACatanGameState::StaticClass();
	PlayerControllerClass = ACatanPlayerController::StaticClass();
	PlayerStateClass = ACatanPlayerState::StaticClass();
	DefaultPawnClass = ACatanCameraPawn::StaticClass();
	HUDClass = ACatanHUD::StaticClass();
	bStartPlayersAsSpectators = false;
}

void ACatanGameMode::BeginPlay()
{
	Super::BeginPlay();

	CreateBoard();
	StartFirstTurnIfPossible();
}

void ACatanGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (ACatanPlayerState* CatanPlayerState = NewPlayer ? NewPlayer->GetPlayerState<ACatanPlayerState>() : nullptr)
	{
		// Turn order is simply join order. That is enough for a local-network prototype.
		CatanPlayerState->SetTurnOrder(GameState ? GameState->PlayerArray.Num() - 1 : INDEX_NONE);
		CatanPlayerState->SetPlayerName(FString::Printf(TEXT("Player %d"), CatanPlayerState->GetTurnOrder() + 1));
	}

	BroadcastStateSummary(TEXT("A new player joined the match."));
	StartFirstTurnIfPossible();
}

bool ACatanGameMode::RollDiceForPlayer(ACatanPlayerController* RequestingPlayer)
{
	ACatanGameState* CatanGameState = GetCatanGameState();
	ACatanPlayerState* PlayerState = RequestingPlayer ? RequestingPlayer->GetPlayerState<ACatanPlayerState>() : nullptr;
	if (!CatanGameState || !PlayerState || CatanGameState->WinningPlayerId != INDEX_NONE)
	{
		return false;
	}

	if (PlayerState->GetTurnOrder() != CatanGameState->CurrentTurnPlayerId || PlayerState->HasRolledThisTurn())
	{
		return false;
	}

	const int32 DiceRoll = FMath::RandRange(1, 6) + FMath::RandRange(1, 6);
	CatanGameState->LastDiceRoll = DiceRoll;
	PlayerState->SetHasRolledThisTurn(true);

	int32 TotalResourcesAwarded = 0;
	for (const FCatanTileState& Tile : CatanGameState->BoardTiles)
	{
		if (Tile.DiceNumber == DiceRoll && Tile.SettlementOwnerPlayerId != INDEX_NONE)
		{
			if (ACatanPlayerState* OwningPlayerState = FindPlayerStateByTurnOrder(Tile.SettlementOwnerPlayerId))
			{
				OwningPlayerState->AddResource(Tile.Resource, 1);
				++TotalResourcesAwarded;
			}
		}
	}

	const FString Message = FString::Printf(TEXT("%s rolled %d and triggered %d production event(s)."), *PlayerState->GetPlayerName(), DiceRoll, TotalResourcesAwarded);
	BroadcastStateSummary(Message);
	return true;
}

bool ACatanGameMode::TryBuildSettlement(ACatanPlayerController* RequestingPlayer, int32 TileId)
{
	ACatanGameState* CatanGameState = GetCatanGameState();
	ACatanPlayerState* PlayerState = RequestingPlayer ? RequestingPlayer->GetPlayerState<ACatanPlayerState>() : nullptr;
	if (!CatanGameState || !PlayerState || CatanGameState->WinningPlayerId != INDEX_NONE)
	{
		return false;
	}

	if (PlayerState->GetTurnOrder() != CatanGameState->CurrentTurnPlayerId)
	{
		return false;
	}

	FCatanTileState* Tile = CatanGameState->BoardTiles.FindByPredicate([TileId](const FCatanTileState& Candidate)
	{
		return Candidate.TileId == TileId;
	});
	if (!Tile || Tile->SettlementOwnerPlayerId != INDEX_NONE || Tile->Resource == ECatanResource::Desert)
	{
		return false;
	}

	// First settlement is intentionally free so every player can get on the board quickly.
	if (PlayerState->GetSettlementsBuilt() > 0 && !PlayerState->SpendSettlementCost())
	{
		BroadcastStateSummary(FString::Printf(TEXT("%s needs 1 Wood, 1 Brick, 1 Wheat, and 1 Sheep to build."), *PlayerState->GetPlayerName()));
		return false;
	}

	Tile->SettlementOwnerPlayerId = PlayerState->GetTurnOrder();
	PlayerState->AwardSettlement();
	++CatanGameState->BoardRevision;

	BroadcastStateSummary(FString::Printf(TEXT("%s built on tile %d (%s / %d)."), *PlayerState->GetPlayerName(), Tile->TileId, *CatanStatics::ResourceToString(Tile->Resource), Tile->DiceNumber));

	if (PlayerState->GetVictoryPoints() >= VictoryPointsToWin)
	{
		AnnounceWinner(PlayerState);
	}

	return true;
}

bool ACatanGameMode::EndTurn(ACatanPlayerController* RequestingPlayer)
{
	ACatanGameState* CatanGameState = GetCatanGameState();
	ACatanPlayerState* PlayerState = RequestingPlayer ? RequestingPlayer->GetPlayerState<ACatanPlayerState>() : nullptr;
	if (!CatanGameState || !PlayerState || PlayerState->GetTurnOrder() != CatanGameState->CurrentTurnPlayerId || CatanGameState->WinningPlayerId != INDEX_NONE)
	{
		return false;
	}

	AdvanceTurn();
	return true;
}

void ACatanGameMode::CreateBoard()
{
	ACatanGameState* CatanGameState = GetCatanGameState();
	if (!CatanGameState || CatanGameState->BoardTiles.Num() > 0)
	{
		return;
	}

	struct FCatanBoardSeed
	{
		int32 Q;
		int32 R;
		ECatanResource Resource;
		int32 DiceNumber;
	};

	// Seven tiles keep the prototype readable while still feeling like a mini-Catan board.
	const TArray<FCatanBoardSeed> Seeds = {
		{0, 0, ECatanResource::Desert, 0},
		{1, 0, ECatanResource::Wood, 5},
		{1, -1, ECatanResource::Brick, 2},
		{0, -1, ECatanResource::Wheat, 6},
		{-1, 0, ECatanResource::Sheep, 3},
		{-1, 1, ECatanResource::Ore, 8},
		{0, 1, ECatanResource::Wood, 9},
		{1, 1, ECatanResource::Brick, 10}
	};

	for (int32 Index = 0; Index < Seeds.Num(); ++Index)
	{
		const FCatanBoardSeed& Seed = Seeds[Index];
		const float X = TileSpacing * (1.5f * Seed.Q);
		const float Y = TileSpacing * (FMath::Sqrt(3.f) * (Seed.R + Seed.Q * 0.5f));

		FCatanTileState Tile;
		Tile.TileId = Index;
		Tile.Resource = Seed.Resource;
		Tile.DiceNumber = Seed.DiceNumber;
		Tile.WorldLocation = FVector(X, Y, 0.f);
		CatanGameState->BoardTiles.Add(Tile);
	}

	++CatanGameState->BoardRevision;
	SpawnedBoardActor = GetWorld()->SpawnActor<ACatanBoardActor>(ACatanBoardActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	BroadcastStateSummary(TEXT("Board created. Use the console commands RollDice, BuildTile <id>, and EndTurn to play."));
}

void ACatanGameMode::StartFirstTurnIfPossible()
{
	ACatanGameState* CatanGameState = GetCatanGameState();
	if (!CatanGameState || CatanGameState->CurrentTurnPlayerId != INDEX_NONE || GameState->PlayerArray.Num() == 0)
	{
		return;
	}

	CatanGameState->CurrentTurnPlayerId = 0;
	if (ACatanPlayerState* FirstPlayer = FindPlayerStateByTurnOrder(0))
	{
		FirstPlayer->ResetForNewTurn();
		BroadcastStateSummary(FString::Printf(TEXT("%s starts the match. Build a free first settlement, then roll dice on later turns."), *FirstPlayer->GetPlayerName()));
	}
}

void ACatanGameMode::AdvanceTurn()
{
	ACatanGameState* CatanGameState = GetCatanGameState();
	if (!CatanGameState || GameState->PlayerArray.Num() == 0)
	{
		return;
	}

	const int32 NextTurnOrder = (CatanGameState->CurrentTurnPlayerId + 1) % GameState->PlayerArray.Num();
	CatanGameState->CurrentTurnPlayerId = NextTurnOrder;
	if (ACatanPlayerState* NextPlayer = FindPlayerStateByTurnOrder(NextTurnOrder))
	{
		NextPlayer->ResetForNewTurn();
		BroadcastStateSummary(FString::Printf(TEXT("It is now %s's turn."), *NextPlayer->GetPlayerName()));
	}
}

ACatanPlayerState* ACatanGameMode::FindPlayerStateByTurnOrder(int32 TurnOrder) const
{
	if (!GameState)
	{
		return nullptr;
	}

	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		if (ACatanPlayerState* CatanPlayerState = Cast<ACatanPlayerState>(PlayerState))
		{
			if (CatanPlayerState->GetTurnOrder() == TurnOrder)
			{
				return CatanPlayerState;
			}
		}
	}

	return nullptr;
}

ACatanGameState* ACatanGameMode::GetCatanGameState() const
{
	return GetGameState<ACatanGameState>();
}

void ACatanGameMode::BroadcastStateSummary(const FString& Message) const
{
	if (ACatanGameState* CatanGameState = GetCatanGameState())
	{
		CatanGameState->AddLogMessage(Message);
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (ACatanPlayerController* Controller = Cast<ACatanPlayerController>(Iterator->Get()))
		{
			Controller->ClientReceiveAnnouncement(Message);
		}
	}
}

void ACatanGameMode::AnnounceWinner(ACatanPlayerState* Winner)
{
	if (ACatanGameState* CatanGameState = GetCatanGameState())
	{
		CatanGameState->WinningPlayerId = Winner ? Winner->GetTurnOrder() : INDEX_NONE;
	}

	BroadcastStateSummary(FString::Printf(TEXT("%s reached %d victory points and wins the match!"), Winner ? *Winner->GetPlayerName() : TEXT("Unknown Player"), VictoryPointsToWin));
}
