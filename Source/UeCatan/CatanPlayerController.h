#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CatanPlayerController.generated.h"

UCLASS()
class UECATAN_API ACatanPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACatanPlayerController();

	virtual void BeginPlay() override;

	/** Exec commands make the prototype usable immediately in PIE multiplayer sessions. */
	UFUNCTION(Exec)
	void RollDice();

	UFUNCTION(Exec)
	void BuildTile(int32 TileId);

	UFUNCTION(Exec)
	void EndTurn();

	UFUNCTION(Client, Reliable)
	void ClientReceiveAnnouncement(const FString& Message);

protected:
	UFUNCTION(Server, Reliable)
	void ServerRollDice();

	UFUNCTION(Server, Reliable)
	void ServerBuildTile(int32 TileId);

	UFUNCTION(Server, Reliable)
	void ServerEndTurn();
};
