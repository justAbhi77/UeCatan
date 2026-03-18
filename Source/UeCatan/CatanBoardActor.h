#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CatanBoardActor.generated.h"

class UStaticMesh;
class UMaterialInterface;
class UTextRenderComponent;
class UStaticMeshComponent;

UCLASS()
class UECATAN_API ACatanBoardActor : public AActor
{
	GENERATED_BODY()

public:
	ACatanBoardActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	void RefreshBoardVisuals();

	UPROPERTY()
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditDefaultsOnly, Category = "Catan")
	TObjectPtr<UStaticMesh> TileMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Catan")
	TObjectPtr<UMaterialInterface> TileMaterial;

	UPROPERTY()
	TArray<TObjectPtr<UActorComponent>> SpawnedVisuals;

	int32 CachedBoardRevision = INDEX_NONE;
};
