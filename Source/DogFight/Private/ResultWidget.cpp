// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultWidget.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Components/Button.h>

void UResultWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GotoPlay->OnClicked.AddDynamic(this, &UResultWidget::GotoPlayClick);
	Quit->OnClicked.AddDynamic(this, &UResultWidget::QuitClick);
}
void UResultWidget::GotoPlayClick()
{
	UGameplayStatics::OpenLevel(GetWorld(),TEXT("Playlevel"));
}
void UResultWidget::QuitClick()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}