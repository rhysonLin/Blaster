#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseMapGameMode.generated.h"

UCLASS()
class BLASTER_API ABaseMapGameMode : public AGameModeBase {

GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};