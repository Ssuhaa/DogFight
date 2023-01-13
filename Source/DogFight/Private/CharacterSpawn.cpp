// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSpawn.h"
#include "../Public/SH_Enemy.h"
#include "Components/SceneComponent.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "SH_EnemyFSM.h"
#include "ItemSpawn.h"
#include <Kismet/GameplayStatics.h>
#include "RIM_Player.h"

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

	AItemSpawn* ItemSpawn = Cast<AItemSpawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AItemSpawn::StaticClass()));
	ARIM_Player* Player = Cast<ARIM_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ARIM_Player::StaticClass()));

	for (int32 i = 0; i < 6; i++) //������ ���͸� Tarray SpawnedEnemy�� ��´�.
	{
		spawnedEnemy.Add(GetWorld()->SpawnActor<ASH_Enemy>(EnemyArray[i], SpawnPosition[i]->GetComponentLocation(), SpawnPosition[i]->GetComponentRotation()));
	}

	for (int32 i = 0; i < spawnedEnemy.Num(); i++) // ������ ���͵� ��ο���
	{
		for (int32 j = 0; j < spawnedEnemy.Num(); j++) //������ ���� �� ���� ������ ��θ� �߰���Ų��.
		{
			if (i != j) //�ڱ� �ڽ��� �����ϰ�
			{
				spawnedEnemy[i]->AddTarget(spawnedEnemy[j]); // �ش� ���ʹ��� Ÿ�ٿ� ������ ���ʹ̵��� �߰��Ѵ�.
				spawnedEnemy[j]->fsm->dieDelegate.AddUObject(spawnedEnemy[i], &ASH_Enemy::RemoveTarget); // Ÿ�ٵ��� dieDelegate��  �ش� ���ʹ� �ȿ� �ִ� RemoveTarget�Լ��� ���ε��Ѵ�.
			}
		}
		spawnedEnemy[i]->AddTarget(Player);
		ItemSpawn->createWeapon.AddUObject(spawnedEnemy[i], &ASH_Enemy::AddTarget); //�����۽��������� createWeapon�� �ش� ���ʹ̾ȿ� �ִ� AddTarget�Լ��� ���ε��Ѵ�.
		ItemSpawn->deleteWeapon.AddUObject(spawnedEnemy[i], &ASH_Enemy::RemoveTarget);//�����۽��������� deletWeapon�� �ش� ���ʹ̾ȿ� �ִ� RemoveTarget�Լ��� ���ε��Ѵ�.
		ASH_Enemy* currentenemy = Cast<ASH_Enemy>(spawnedEnemy[i]);
		currentenemy->fsm->RandomTarget();
	}



}

// Called every frame
void ACharacterSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

