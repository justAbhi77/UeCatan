#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CatanTypes.h"
#include "CatanPlayerState.generated.h"

UCLASS()
class UECATAN_API ACatanPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACatanPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetTurnOrder(int32 InTurnOrder);
	int32 GetTurnOrder() const { return TurnOrder; }

	void AddResource(ECatanResource Resource, int32 Amount);
	bool CanAffordSettlement() const;
	bool SpendSettlementCost();
	void AwardSettlement();
	void ResetForNewTurn();

	const FCatanResourceSet& GetResources() const { return Resources; }
	int32 GetVictoryPoints() const { return VictoryPoints; }
	int32 GetSettlementsBuilt() const { return SettlementsBuilt; }
	bool HasRolledThisTurn() const { return bHasRolledThisTurn; }
	void SetHasRolledThisTurn(bool bInHasRolledThisTurn) { bHasRolledThisTurn = bInHasRolledThisTurn; }

protected:
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Catan")
	FCatanResourceSet Resources;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Catan")
	int32 VictoryPoints = 0;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Catan")
	int32 SettlementsBuilt = 0;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Catan")
	int32 TurnOrder = INDEX_NONE;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Catan")
	bool bHasRolledThisTurn = false;
};
