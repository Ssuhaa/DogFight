// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Timer.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UTimer : public UUserWidget
{
	GENERATED_BODY()
protected:
	//BeginPlay 와 같은 동작
	virtual void NativeConstruct() override;

public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Min;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Sec;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* VerticalDown;

	void UpdateSetText(int32 min, int32 sec);
};
