#include "CatanCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"

ACatanCameraPawn::ACatanCameraPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(SceneRoot);
	SpringArm->TargetArmLength = 2200.f;
	SpringArm->SetRelativeRotation(FRotator(-65.f, 0.f, 0.f));
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	SetActorLocation(FVector(150.f, 150.f, 0.f));
}
