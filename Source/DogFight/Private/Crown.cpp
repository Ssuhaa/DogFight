// Fill out your copyright notice in the Description page of Project Settings.


#include "Crown.h"

// Sets default values
ACrown::ACrown()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Crown = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrownMesh"));
	Crown->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder <UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/Geometry/Mesh/Crown_01.Crown_01'"));
	if (tempMesh.Succeeded())
	{
		Crown->SetStaticMesh(tempMesh.Object);
	}
	Crown->SetRelativeScale3D(FVector(7));

}

// Called when the game starts or when spawned
void ACrown::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACrown::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Runtime += DeltaTime;
	float sinRuntime = FMath::Sin(Runtime*Speed);
	float locationvalue = sinRuntime*Length;
	Crown->AddRelativeRotation(FRotator(0,1,0));
	Crown->SetRelativeLocation(FVector(GetActorLocation().X, GetActorLocation().Y,locationvalue+100));
}

