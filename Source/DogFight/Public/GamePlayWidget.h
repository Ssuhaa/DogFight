// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UGamePlayWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	//BeginPlay 와 같은 동작
	virtual void NativeConstruct() override;

public:


	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* PlayStartButton;
	
	UFUNCTION()
	void play();
};
