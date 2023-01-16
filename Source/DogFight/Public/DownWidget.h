// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DownWidget.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UDownWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* NameText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* DownCountText;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 DownCount;
	//

};
