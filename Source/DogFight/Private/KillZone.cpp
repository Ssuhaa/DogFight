// Fill out your copyright notice in the Description page of Project Settings.


#include "KillZone.h"
#include <Components/BoxComponent.h>
#include "SH_Enemy.h"
#include "SH_Player.h"
#include "SH_EnemyFSM.h"
#include "Weapon.h"


// Sets default values
AKillZone::AKillZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box")); 
	SetRootComponent(compBox);
	compBox->SetBoxExtent(FVector(2500, 2500, 200));
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
	
	Player = Cast<ASH_Player>(OtherActor);
	if (Player != nullptr)
	{
		//킬존에 플레이어가 닿이면 죽는 코드
	}
	Enemy = Cast<ASH_Enemy>(OtherActor);
	if (Enemy != nullptr)
	{
		Enemy->fsm->stateChangeMontage(EEnemyState::Die,TEXT("Die"));
	}
	weapon = Cast<AWeapon>(OtherActor);
	if (weapon != nullptr)
	{
		weapon->Destroy();
	}
	

}