// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSpawn.h"
#include "../Public/SH_Enemy.h"
#include "Components/SceneComponent.h"
#include <GameFramework/CharacterMovementComponent.h>

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
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	for (int32 i = 0; i < 6; i++)
	{
		FString str = FString::Printf(TEXT("Position%d"), i);
		USceneComponent* temp2 = CreateDefaultSubobject<USceneComponent>(*str);
		SpawnPosition.Add(temp2);
		SpawnPosition[i]->SetupAttachment(RootComponent);
		FVector V = FVector::ForwardVector;
		V = V.RotateAngleAxis(i * (360 / 7), FVector::UpVector);
		SpawnPosition[i]->SetRelativeLocation(V * SpawnCircleSize);
	}
	
}

// Called when the game starts or when spawned
void ACharacterSpawn::BeginPlay()
{
	Super::BeginPlay();
	for (int32 i = 0; i < 6; i++)
	{
		GetWorld()->SpawnActor<ASH_Enemy>(EnemyArray[i], SpawnPosition[i]->GetComponentLocation(), SpawnPosition[i]->GetComponentRotation());
	}
}

// Called every frame
void ACharacterSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

