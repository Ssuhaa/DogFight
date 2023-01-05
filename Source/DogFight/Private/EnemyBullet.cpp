// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "SH_Enemy.h"

// Sets default values
AEnemyBullet::AEnemyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	compCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = compCollision;
	compCollision->SetSphereRadius(13);
	compCollision->SetCollisionProfileName(TEXT("Weapon"));

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	compMesh->SetupAttachment(compCollision);
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/Geometry/Carrot.Carrot'"));
	if (tempMesh.Succeeded())
	{
		compMesh->SetStaticMesh(tempMesh.Object);
	}
	compMesh->SetRelativeRotation(FRotator(110,0,90));
	compMesh->SetRelativeScale3D(FVector(2));
	compMesh->SetCollisionProfileName(TEXT("NoCollision"));

	compMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	compMovement->SetUpdatedComponent(compCollision);
	compMovement->InitialSpeed = 1000; 
	compMovement->MaxSpeed = 1000;
	compMovement->bShouldBounce = true;
	compMovement->Bounciness = 0.8f; 

	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();
	compCollision->OnComponentBeginOverlap.AddDynamic(this,&AEnemyBullet::BulletBeginOverlap);
	
}

// Called every frame
void AEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBullet::BulletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains(TEXT("Player"))) //���࿡ �ε��� OtherActor �߿� �̸��� enemy�� �����ϰ�������
	{
		UE_LOG(LogTemp,Warning,TEXT("Bullet Hit player"));
		Destroy();
	}
	else if(OtherActor->GetName().Contains(TEXT("Enemy"))) //���࿡ �ε��� OtherActor �߿� �̸��� enemy�� �����ϰ�������
	{
		
		if (OtherActor != GetOwner())
		{
		UE_LOG(LogTemp, Warning, TEXT("Bullet Hit Enemy"));
		Destroy();

		}
	}
}
