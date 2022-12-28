// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* GotoPlay;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Quit;

protected:
	//BeginPlay 와 같은 동작
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void GotoPlayClick();
	UFUNCTION()
	void QuitClick();
};
