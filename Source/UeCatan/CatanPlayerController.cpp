#include "CatanPlayerController.h"
#include "CatanGameMode.h"
#include "Engine/Engine.h"

ACatanPlayerController::ACatanPlayerController()
{
	bReplicates = true;
	bShowMouseCursor = true;
}

void ACatanPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		ClientMessage(TEXT("Welcome to UE Catan Prototype. Open the console and use: RollDice, BuildTile <TileId>, EndTurn"));
	}
}

void ACatanPlayerController::RollDice()
{
	ServerRollDice();
}

void ACatanPlayerController::BuildTile(int32 TileId)
{
	ServerBuildTile(TileId);
}

void ACatanPlayerController::EndTurn()
{
	ServerEndTurn();
}

void ACatanPlayerController::ClientReceiveAnnouncement_Implementation(const FString& Message)
{
	ClientMessage(Message);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, Message);
	}
}

void ACatanPlayerController::ServerRollDice_Implementation()
{
	if (ACatanGameMode* CatanGameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACatanGameMode>() : nullptr)
	{
		if (!CatanGameMode->RollDiceForPlayer(this))
		{
			ClientMessage(TEXT("RollDice failed. Make sure it is your turn and you have not rolled already."));
		}
	}
}

void ACatanPlayerController::ServerBuildTile_Implementation(int32 TileId)
{
	if (ACatanGameMode* CatanGameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACatanGameMode>() : nullptr)
	{
		if (!CatanGameMode->TryBuildSettlement(this, TileId))
		{
			ClientMessage(TEXT("BuildTile failed. The tile may be occupied, desert, too expensive, or not on your turn."));
		}
	}
}

void ACatanPlayerController::ServerEndTurn_Implementation()
{
	if (ACatanGameMode* CatanGameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACatanGameMode>() : nullptr)
	{
		if (!CatanGameMode->EndTurn(this))
		{
			ClientMessage(TEXT("EndTurn failed. Make sure it is your turn."));
		}
	}
}
