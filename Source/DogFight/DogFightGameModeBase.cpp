// Copyright Epic Games, Inc. All Rights Reserved.


#include "DogFightGameModeBase.h"
#include "Timer.h"
#include <Kismet/GameplayStatics.h>
#include "SuccessWidget.h"


void ADogFightGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	TimeWG = CreateWidget<UTimer>(GetWorld(), Timer);
	successWG = CreateWidget<USuccessWidget>(GetWorld(), successUI);
}

ADogFightGameModeBase::ADogFightGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<UTimer> tempTimeWG(TEXT("WidgetBlueprint'/Game/BluePrint/BP_Timer.BP_Timer_C'"));
	if (tempTimeWG.Succeeded())
	{
		Timer = tempTimeWG.Class;
	}
	ConstructorHelpers::FClassFinder<USuccessWidget> tempSuccessWG(TEXT("WidgetBlueprint'/Game/BluePrint/BP_Succes.BP_Succes_C'"));
	if (tempSuccessWG.Succeeded())
	{
		successUI = tempSuccessWG.Class;
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
				if (successWG->IsInViewport() == false)
				{
					if (TimeWG->IsInViewport() == true)
					{
						successWG->AddToViewport();
						TimeWG->RemoveFromParent();
					}
				}
				else
				{
					openleveltime += DeltaTime;
					if (openleveltime > 2)
					{
						UGameplayStatics::OpenLevel(GetWorld(), "Result");
					}

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