// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DogFightGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API ADogFightGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADogFightGameModeBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float currenttime = 0;
	int32 SuccessMintime = 5;
	float SuccessSectime = 0;
	float openleveltime = 0;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UTimer> Timer;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USuccessWidget> successUI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USuccessWidget* successWG;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	class UTimer* TimeWG;
};
