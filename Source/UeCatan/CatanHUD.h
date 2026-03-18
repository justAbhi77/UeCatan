#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CatanHUD.generated.h"

UCLASS()
class UECATAN_API ACatanHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;
};
