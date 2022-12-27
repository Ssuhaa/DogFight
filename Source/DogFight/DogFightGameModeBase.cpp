// Copyright Epic Games, Inc. All Rights Reserved.


#include "DogFightGameModeBase.h"
#include "Timer.h"
#include <Kismet/GameplayStatics.h>

void ADogFightGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	TimeWG = CreateWidget<UTimer>(GetWorld(),Timer);
	//TimeWG->AddToViewport();
}

ADogFightGameModeBase::ADogFightGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<UTimer> tempWG(TEXT("WidgetBlueprint'/Game/BluePrint/BP_Timer.BP_Timer_C'"));
	if (tempWG.Succeeded())
	{
		Timer = tempWG.Class;
	}
}

void ADogFightGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	currenttime += DeltaTime;
	if (currenttime > 1)
	{
		if (SuccessSectime > 0)
		{
			SuccessSectime--;
			currenttime = 0;
		}
		else if (SuccessMintime >= 0)
		{
			if (SuccessMintime == 0)
			{
				TimeWG->RemoveFromParent();
				if (currenttime > 2)
				{
					//!!!!!!!!성공 위젯을 띄우고
					//!!!!!!!!커런트 타임 리셋
					//!!!!!!!!2초 뒤에
					//!!!!!!!!오픈레벨
					UE_LOG(LogTemp,Warning,TEXT("TIME OUT!"));
					UGameplayStatics::OpenLevel(GetWorld(), "ResultLevel");
				}
	
			}
			else
			{
				SuccessMintime--;
				SuccessSectime = 60;
				currenttime = 0;
			}
		}
	}

	TimeWG->UpdateSetText(SuccessMintime, int32(SuccessSectime));
	
	
}