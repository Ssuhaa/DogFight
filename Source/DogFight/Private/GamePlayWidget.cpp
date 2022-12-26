// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayWidget.h"
#include <Kismet/GameplayStatics.h>
#include <Components/Button.h>

void UGamePlayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayStartButton->OnClicked.AddDynamic(this, &UGamePlayWidget::play);
}

void  UGamePlayWidget::play()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMap");
}