
// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawn.h"
#include "Weapon.h"
#include "GunWeapon.h"

// Sets default values
AItemSpawn::AItemSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	itemSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ItemSpawnLocation"));
	RootComponent = itemSpawnLocation;

	ConstructorHelpers::FClassFinder <ALollipopWeapon> tempLollipop(TEXT("Class'/Script/DogFight.LollipopWeapon_C'"));
	if (tempLollipop.Succeeded())
	{
		Loliipop = tempLollipop.Class;
	}
	ConstructorHelpers::FClassFinder <AGunWeapon> tempGun(TEXT("Class'/Script/DogFight.GunWeapon_C'"));
	if (tempLollipop.Succeeded())
	{
		Gun = tempGun.Class;
	}

	ItemArray.Add(Loliipop);
	ItemArray.Add(Gun);
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
		int32 index = FMath::RandRange(0, ItemArray.Num());
		GetWorld()->SpawnActor<AWeapon>(ItemArray[index]);
		CurrentTime = 0;
	}

}

