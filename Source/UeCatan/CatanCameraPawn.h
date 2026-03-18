#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CatanCameraPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class UECATAN_API ACatanCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ACatanCameraPawn();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Catan")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Catan")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Catan")
	TObjectPtr<UCameraComponent> Camera;
};
