
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

	ConstructorHelpers::FClassFinder <AGunWeapon> tempGun(TEXT("Class'/Script/DogFight.GunWeapon'"));
	if (tempGun.Succeeded())
	{
		ItemArray.Add(tempGun.Class);
	}
	ConstructorHelpers::FClassFinder <ALollipopWeapon> tempLollipop(TEXT("Class'/Script/DogFight.LollipopWeapon'"));
	if (tempLollipop.Succeeded())
	{
		ItemArray.Add(tempLollipop.Class);
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
		CreateWeapon(index, itemSpawnLocation->GetComponentLocation(), itemSpawnLocation->GetComponentRotation());
		CurrentTime = 0;		
	}
}

void AItemSpawn::CreateWeapon(int32 index, FVector SpwanLocation, FRotator SpwanRotation) //웨폰 스폰 하고 스폰된 웨폰을 에너미타겟의 addTarget에 전달. 
{
	AWeapon* weapon = GetWorld()->SpawnActor<AWeapon>(ItemArray[index], SpwanLocation, SpwanRotation);
	createWeapon.Broadcast(weapon);
}

void AItemSpawn::DeleteWeapon(AActor* actor) // 받은액터를 에너미타겟의 removeTarget에 전달.
{
	deleteWeapon.Broadcast(actor);
}
