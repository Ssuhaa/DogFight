// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>
#include "RIM_Player.h"
#include "SH_Enemy.h"
#include "SH_EnemyFSM.h"
#include "EnemyAnim.h"
#include <Engine/SkeletalMeshSocket.h>
#include "SH_Player.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//WeaponMesh
	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = compMesh;
	compMesh->SetupAttachment(compCollision);
	compMesh->SetCollisionProfileName(TEXT("BlockAll"));
	compMesh->SetSimulatePhysics(true);

	//WeaponCollison
	compCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	compCollision->SetupAttachment(compMesh);
	compCollision->SetBoxExtent(FVector(40));
	compCollision->SetCollisionProfileName(TEXT("OverlapAll"));
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	compCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::collisionBeginOverlap);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::EnableInput(class APlayerController* PlayerController)
{
	Super::EnableInput(PlayerController);
	PlayerController->InputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AWeapon::BindGetWeapon);
}

void AWeapon::collisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	overlapActor = OtherActor;
	if (OtherActor->GetName().Contains(TEXT("Player")))
	{
		EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
	else if (OtherActor->GetName().Contains(TEXT("Enemy")))
	{
		BindGetWeapon();
	}
}

void AWeapon::BindGetWeapon()
{
	
	GetWeapon(overlapActor);
}

void AWeapon::GetWeapon(AActor* Actor)
{
	player = Cast<ARIM_Player>(Actor);
	if (player != nullptr)
	{
		player->VisibleGun();
		Destroy();
	}
	Enemy = Cast<ASH_Enemy>(Actor);
	if (Enemy != nullptr)
	{
		if(Enemy->fsm->anim->isGunget == false && Enemy->fsm->anim->isLollipopget == false)
		{
			Enemy->GetEnemyWeapon(compMesh->GetStaticMesh(), Soketname);
			Enemy->fsm->stateChange(EEnemyState::Pickup);
			if (Soketname == TEXT("Lollipop"))
			{
				Enemy->fsm->anim->isLollipopget = true;
			}
			else if((Soketname == TEXT("Gun")))
			{
				Enemy->fsm->anim->isGunget = true;
			}
			Destroy();
		}
	}

}
