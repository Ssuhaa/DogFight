// Fill out your copyright notice in the Description page of Project Settings.


#include "Timer.h"
#include <Components/TextBlock.h>

void UTimer ::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UTimer::UpdateSetText(int32 min, int32 sec)
{
	Min->SetText(FText::AsNumber(min));
	Sec->SetText(FText::AsNumber(sec));
}