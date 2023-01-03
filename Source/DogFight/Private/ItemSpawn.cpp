
// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawn.h"
#include "Weapon.h"
#include "GunWeapon.h"
#include "LollipopWeapon.h"

// Sets default values
AItemSpawn::AItemSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	itemSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ItemSpawnLocation"));
	RootComponent = itemSpawnLocation;

	ConstructorHelpers::FClassFinder <ALollipopWeapon> tempLollipop(TEXT("Class'/Script/DogFight.LollipopWeapon'"));
	if (tempLollipop.Succeeded())
	{
		ItemArray.Add(tempLollipop.Class);
	}
	ConstructorHelpers::FClassFinder <AGunWeapon> tempGun(TEXT("Class'/Script/DogFight.GunWeapon'"));
	if (tempLollipop.Succeeded())
	{
		ItemArray.Add(tempGun.Class);
	}
	
}

// Called when the game starts or when spawned
void AItemSpawn::BeginPlay()
{
	Super::BeginPlay();
	 
}

// Called every frame
void AItemSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentTime+=DeltaTime;
	if (CurrentTime > SpawnTime)
	{
		int32 index = FMath::RandRange(0, ItemArray.Num()-1);
		GetWorld()->SpawnActor<AWeapon>(ItemArray[index], itemSpawnLocation->GetRelativeTransform());
		CurrentTime = 0;
	}

}

