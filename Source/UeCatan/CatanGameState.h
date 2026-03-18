#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CatanTypes.h"
#include "CatanGameState.generated.h"

UCLASS()
class UECATAN_API ACatanGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACatanGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddLogMessage(const FString& Message);
	const TArray<FString>& GetMatchLog() const { return MatchLog; }
	const FCatanTileState* FindTile(int32 TileId) const;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Catan")
	TArray<FCatanTileState> BoardTiles;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Catan")
	int32 CurrentTurnPlayerId = INDEX_NONE;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Catan")
	int32 LastDiceRoll = 0;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Catan")
	int32 WinningPlayerId = INDEX_NONE;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Catan")
	int32 BoardRevision = 0;

protected:
	/** Small replicated text feed so every client sees what happened without needing widgets. */
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Catan")
	TArray<FString> MatchLog;
};
