#include "CatanBoardActor.h"
#include "CatanGameState.h"
#include "CatanTypes.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

ACatanBoardActor::ACatanBoardActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (CylinderMesh.Succeeded())
	{
		TileMesh = CylinderMesh.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BasicMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (BasicMaterial.Succeeded())
	{
		TileMaterial = BasicMaterial.Object;
	}
}

void ACatanBoardActor::BeginPlay()
{
	Super::BeginPlay();
	RefreshBoardVisuals();
}

void ACatanBoardActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const ACatanGameState* CatanGameState = GetWorld() ? GetWorld()->GetGameState<ACatanGameState>() : nullptr;
	if (CatanGameState && CachedBoardRevision != CatanGameState->BoardRevision)
	{
		RefreshBoardVisuals();
	}
}

void ACatanBoardActor::RefreshBoardVisuals()
{
	const ACatanGameState* CatanGameState = GetWorld() ? GetWorld()->GetGameState<ACatanGameState>() : nullptr;
	if (!CatanGameState)
	{
		return;
	}

	for (UActorComponent* Component : SpawnedVisuals)
	{
		if (Component)
		{
			Component->DestroyComponent();
		}
	}
	SpawnedVisuals.Reset();

	for (const FCatanTileState& Tile : CatanGameState->BoardTiles)
	{
		UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this);
		MeshComponent->SetupAttachment(SceneRoot);
		MeshComponent->SetStaticMesh(TileMesh);
		MeshComponent->SetMaterial(0, TileMaterial);
		MeshComponent->SetWorldScale3D(FVector(2.1f, 2.1f, 0.2f));
		MeshComponent->SetRelativeLocation(Tile.WorldLocation);
		MeshComponent->SetMobility(EComponentMobility::Movable);
		MeshComponent->RegisterComponent();
		SpawnedVisuals.Add(MeshComponent);

		UTextRenderComponent* TextComponent = NewObject<UTextRenderComponent>(this);
		TextComponent->SetupAttachment(SceneRoot);
		TextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
		TextComponent->SetWorldSize(45.f);
		TextComponent->SetTextRenderColor(CatanStatics::ResourceToColor(Tile.Resource));
		TextComponent->SetRelativeLocation(Tile.WorldLocation + FVector(0.f, 0.f, 120.f));
		TextComponent->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

		const FString OwnerText = Tile.SettlementOwnerPlayerId == INDEX_NONE
			? TEXT("Open")
			: FString::Printf(TEXT("P%d"), Tile.SettlementOwnerPlayerId + 1);
		const FString Label = FString::Printf(TEXT("[%d] %s\nRoll %d\n%s"), Tile.TileId, *CatanStatics::ResourceToString(Tile.Resource), Tile.DiceNumber, *OwnerText);
		TextComponent->SetText(FText::FromString(Label));
		TextComponent->RegisterComponent();
		SpawnedVisuals.Add(TextComponent);
	}

	CachedBoardRevision = CatanGameState->BoardRevision;
}
