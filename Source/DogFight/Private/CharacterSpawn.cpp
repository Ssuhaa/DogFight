// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSpawn.h"
#include "../Public/SH_Enemy.h"
#include "Components/SceneComponent.h"

// Sets default values
ACharacterSpawn::ACharacterSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FClassFinder<ASH_Enemy> temp(TEXT("Class'/Script/DogFight.SH_Enemy'"));
	if (temp.Succeeded())
	{
		for (int32 i = 0; i < 6; i++)
		{
			EnemyArray.Add(temp.Class);
		}
	}
	

	
// 	for (int32 i = 0; i < 6; i++)
// 	{
// 		FString str = FString::Printf(TEXT("Position%d"), i);
// 		USceneComponent* temp2 = CreateDefaultSubobject<USceneComponent>(*str);
// 		SpawnPosition.Add(temp2);
// 		SpawnPosition[i]->SetupAttachment(RootComponent);
// 
// 	}


}

// Called when the game starts or when spawned
void ACharacterSpawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

