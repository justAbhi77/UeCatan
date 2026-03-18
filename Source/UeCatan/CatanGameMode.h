#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CatanTypes.h"
#include "CatanGameMode.generated.h"

class ACatanPlayerController;
class ACatanPlayerState;
class ACatanGameState;
class ACatanBoardActor;

UCLASS()
class UECATAN_API ACatanGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACatanGameMode();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	bool RollDiceForPlayer(ACatanPlayerController* RequestingPlayer);
	bool TryBuildSettlement(ACatanPlayerController* RequestingPlayer, int32 TileId);
	bool EndTurn(ACatanPlayerController* RequestingPlayer);

protected:
	void CreateBoard();
	void StartFirstTurnIfPossible();
	void AdvanceTurn();
	ACatanPlayerState* FindPlayerStateByTurnOrder(int32 TurnOrder) const;
	ACatanGameState* GetCatanGameState() const;
	void BroadcastStateSummary(const FString& Message) const;
	void AnnounceWinner(ACatanPlayerState* Winner);

	UPROPERTY(EditDefaultsOnly, Category = "Catan")
	int32 VictoryPointsToWin = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Catan")
	float TileSpacing = 260.f;

	UPROPERTY()
	TObjectPtr<ACatanBoardActor> SpawnedBoardActor;
};
