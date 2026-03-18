#include "CatanHUD.h"
#include "CatanGameState.h"
#include "CatanPlayerState.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"

void ACatanHUD::DrawHUD()
{
	Super::DrawHUD();

	const ACatanGameState* CatanGameState = GetWorld() ? GetWorld()->GetGameState<ACatanGameState>() : nullptr;
	const ACatanPlayerState* LocalPlayerState = GetOwningPlayerController() ? GetOwningPlayerController()->GetPlayerState<ACatanPlayerState>() : nullptr;
	if (!Canvas || !CatanGameState || !LocalPlayerState)
	{
		return;
	}

	float Y = 30.f;
	const float X = 30.f;
	const UFont* Font = GEngine ? GEngine->GetSmallFont() : nullptr;

	const FString Header = FString::Printf(TEXT("UE Catan Prototype | Turn: P%d | Last Roll: %d | Victory: %d"), CatanGameState->CurrentTurnPlayerId + 1, CatanGameState->LastDiceRoll, LocalPlayerState->GetVictoryPoints());
	Canvas->DrawText(Font, Header, X, Y);
	Y += 24.f;
	Canvas->DrawText(Font, FString::Printf(TEXT("Resources: %s"), *LocalPlayerState->GetResources().ToDisplayString()), X, Y);
	Y += 24.f;
	Canvas->DrawText(Font, TEXT("Commands: RollDice | BuildTile <TileId> | EndTurn"), X, Y);
	Y += 30.f;
	Canvas->DrawText(Font, TEXT("Recent Match Log:"), X, Y);
	Y += 22.f;

	for (const FString& Line : CatanGameState->GetMatchLog())
	{
		Canvas->DrawText(Font, Line, X, Y);
		Y += 18.f;
	}
}
