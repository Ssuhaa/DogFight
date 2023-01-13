// Copyright Epic Games, Inc. All Rights Reserved.


#include "DogFightGameModeBase.h"
#include "Timer.h"
#include <Kismet/GameplayStatics.h>
#include "SuccessWidget.h"
#include "SH_Enemy.h"
#include "SH_EnemyFSM.h"
#include "FailWidget.h"


void ADogFightGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	TimeWG = CreateWidget<UTimer>(GetWorld(), Timer);
	successWG = CreateWidget<USuccessWidget>(GetWorld(), successUI);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASH_Enemy::StaticClass(), enemyarray);

	//failWG = CreateWidget<UFailWidget>(GetWorld(), successUI); //���� �߰�


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
// 	ConstructorHelpers::FClassFinder<UFailWidget> tempFailWG(TEXT("WidgetBlueprint'/Game/BluePrint/BP_Fail.BP_Fail_C'")); //���� �߰�
// 	if (tempFailWG.Succeeded()) //���� �߰�
// 	{
// 		failUI = tempFailWG.Class; //���� �߰�
// 	}
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
	checkEnemyState();
	if (EnemyAllDie == true)
	{
		if (successWG->IsInViewport() == false)
		{
			if (TimeWG->IsInViewport() == true)
			{
				successWG->AddToViewport();
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



}

void ADogFightGameModeBase::checkEnemyState() //���ʹ̰� �� �׾����� üũ�ϴ� �Լ�
{

	for (int32 i = 0; i < enemyarray.Num(); i++)
	{
		currEnemy = Cast<ASH_Enemy>(enemyarray[i]);
		if (currEnemy->fsm->mState != EEnemyState::Die)
		{
			EnemyAllDie = false; return;
		}
		else
		{
			EnemyAllDie = true;
		}
	}
}


//���з��� �����