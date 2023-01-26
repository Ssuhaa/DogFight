// Fill out your copyright notice in the Description page of Project Settings.


#include "KillZone.h"
#include <Components/BoxComponent.h>
#include "SH_Enemy.h"
#include "SH_EnemyFSM.h"
#include "Weapon.h"
#include "RIM_Player.h"
#include "ItemSpawn.h"
#include <Kismet/GameplayStatics.h>
#include "FogManager.h"

// Sets default values
AKillZone::AKillZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box")); 
	SetRootComponent(compBox);
	compBox->SetBoxExtent(FVector(2500, 2500, 200));
	compBox->SetCollisionProfileName(TEXT("OverlapAll"));
}

// Called when the game starts or when spawned
void AKillZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKillZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKillZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AItemSpawn* ItemSpawn = Cast<AItemSpawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AItemSpawn::StaticClass()));
	AFogManager* FogManager = Cast<AFogManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AFogManager::StaticClass())); //FogManager 이 무엇인지 알려줘야 한다

	//[정림 추가]
	//테스트1(XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)
	//float moveDist = FVector::Distance(FogManager->GetActorLocation(), OtherActor->GetActorLocation()); //AliveZone 원점(위치)과 킬존에 부딪힌 액터들(위치)의 거리. 간격
	//FVector moveDist = OtherActor->GetActorLocation() - FogManager->GetActorLocation(); //킬존에 부딪힌 액터 위치 - AliveZone 위치
	//if (moveDist.Length() > FogManager->aliveRange) //액터들이 AliveZone 범위를 벗어나면
	
	//테스트2
	if (FogManager->isAlive == false) //★★★이거 오류남. 멤버가 없다는데 뭐야!!!!!!!!!!!!!!!!!!!!!!
	{
		UE_LOG(LogTemp, Error, TEXT("chicken111111111111111111111111111111111111111111111"));

		//[수하님 코드]
		Player = Cast<ARIM_Player>(OtherActor);
		if (Player != nullptr) // 킬존에 닿인 것이 플레이어일 때
		{
			//킬존에 플레이어가 닿이면 죽는 코드
			Player->Die();
		}
		Enemy = Cast<ASH_Enemy>(OtherActor);
		if (Enemy != nullptr)
		{
			Enemy->fsm->stateChangeMontage(EEnemyState::Die,TEXT("Die"));
		}
		weapon = Cast<AWeapon>(OtherActor);
		if (weapon != nullptr)
		{
			ItemSpawn->DeleteWeapon(weapon);
			weapon->Destroy();
		}


	}
}