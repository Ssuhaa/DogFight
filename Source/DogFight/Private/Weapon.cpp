// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>
#include "RIM_Player.h"
#include "SH_Enemy.h"
#include "SH_EnemyFSM.h"
#include "EnemyAnim.h"
#include "ItemSpawn.h"


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
	compCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::collisionEndOverlap);

}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::collisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains(TEXT("Player")))
	{
		player = Cast<ARIM_Player>(OtherActor);
		EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		

	}
	else if (OtherActor->GetName().Contains(TEXT("Enemy")))
	{
		Enemy = Cast<ASH_Enemy>(OtherActor);
		GetWeapon();
	}
}
void AWeapon::collisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->InputComponent->RemoveActionBinding("Pickup", IE_Pressed);
}


void AWeapon::EnableInput(class APlayerController* PlayerController)
{
	Super::EnableInput(PlayerController);
	PlayerController->InputComponent->BindAction(TEXT("Pickup"), IE_Pressed, this, &AWeapon::GetWeapon);
	
}



void AWeapon::GetWeapon()
{
	AItemSpawn* ItemSpawn = Cast<AItemSpawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AItemSpawn::StaticClass()));
	if (player != nullptr)
	{
		if(player->compMeshLollipop->IsVisible() || player->compMeshGun->IsVisible()) return;
			if (WeaponType == EWeaponType::Gun) //���� �̸��� �ؽ�Ʈ�� Gun �̸�
			{		
				UGameplayStatics::GetPlayerController(GetWorld(), 0)->InputComponent->RemoveActionBinding("Pickup", IE_Pressed);
				player->VisibleGun(); //�÷��̾��� Gun �޽ð� ���̰� �Ѵ�. �÷��̾ Gun �� ��� �ְ� �Ѵ�. ---> �÷��̾� Gun ������ �Լ� ȣ��
				ItemSpawn->DeleteWeapon(this);
				UE_LOG(LogTemp, Error, TEXT("Player Gun Pickup!")); //Ȯ�ο� �ؽ�Ʈ ���;
				Destroy(); //�ٴ��� Gun �� �ı��Ѵ�
	
			}
			else if (WeaponType == EWeaponType::Lollipop) //���� �̸��� �ؽ�Ʈ�� Lollipop �̸�
			{
				UGameplayStatics::GetPlayerController(GetWorld(), 0)->InputComponent->RemoveActionBinding("Pickup", IE_Pressed);
				player->VisibleLollipop(); //�÷��̾��� Lollipop �޽ð� ���̰� �Ѵ�. �÷��̾ Lollipop �� ��� �ְ� �Ѵ�. ---> �÷��̾� Lollipop ������ �Լ� ȣ��
				ItemSpawn->DeleteWeapon(this);
				UE_LOG(LogTemp, Error, TEXT("Player Lollipop Pickup!")); //Ȯ�ο� �ؽ�Ʈ ���
				Destroy(); //�ٴ��� Lollipop �� �ı��Ѵ�
			}
	}
	if (Enemy != nullptr)
	{
		if (Enemy->fsm->mState == EEnemyState::Damage || Enemy->fsm->mState == EEnemyState::Down || Enemy->fsm->mState == EEnemyState::Die || Enemy->fsm->mState == EEnemyState::Attack) return;
		if (Enemy->fsm->anim->isGunget == false && Enemy->fsm->anim->isLollipopget == false)
		{

			Enemy->GetEnemyWeapon(compMesh->GetStaticMesh(), Soketname);
			Enemy->fsm->stateChange(EEnemyState::Pickup);
			if (WeaponType == EWeaponType::Lollipop)
			{
				Enemy->fsm->anim->isLollipopget = true;
			}
			else if (WeaponType == EWeaponType::Gun)
			{
				Enemy->fsm->anim->isGunget = true;
			}
			ItemSpawn->DeleteWeapon(this);
			Destroy();
		}
	}

}
