// Fill out your copyright notice in the Description page of Project Settings.


#include "FogManager.h"
#include <Components/StaticMeshComponent.h>
#include <Components/ShapeComponent.h>
#include "KillZone.h"
#include <GameFramework/Actor.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/Character.h>
#include "Weapon.h"
#include "RIM_Player.h"
#include "SH_Enemy.h"
#include "SH_EnemyFSM.h"
#include "ItemSpawn.h"

// Sets default values
AFogManager::AFogManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	compMeshAlive = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AliveMesh"));
	RootComponent = compMeshAlive;
	compMeshAlive->SetRelativeLocation(FVector(110, -400, 0));
	compMeshAlive->SetRelativeScale3D(FVector(scale));
	compMeshAlive->SetCollisionProfileName(TEXT("OverlapAll"));
	//�޽�, ���׸��� �������Ʈ���� ����
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMeshAlive(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (tempMeshAlive.Succeeded())
	{
		compMeshAlive->SetStaticMesh(tempMeshAlive.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterial> tempM(TEXT("Material'/Game/InfinityBladeEffects/Effects/FX/M_FogPlane02.M_FogPlane02'"));
	if (tempM.Succeeded())
	{
		compMeshAlive->SetMaterial(0, tempM.Object);
	}
}

// Called when the game starts or when spawned
void AFogManager::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AFogManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AliveZone();

}

void AFogManager::AliveZone()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	
	if (scale < 5)
	{
		scaleSpeed = 0;
	}
	else
	{
		if (currentTime < 30)
		{
			scale -= scaleSpeed;
		}
		else
		{
			delayTime += GetWorld()->DeltaTimeSeconds;
			if (delayTime > 10)
			{
				currentTime = 0;
				delayTime = 0;
			}
		}
	}

	compMeshAlive->SetRelativeScale3D(FVector(scale));

	aliveRange = scale * 100 / 2; //AliveZone �� ������. AliveZone ����. �������� ���������� ��ġ? �Ÿ�?
	//aliveRange = r; //AliveZone ����. �������� ���������� ��ġ? �Ÿ�? �ڡڡ�400 �� �ƴ϶� ��ġ Y ��!!!
	FindActorPos();
}


void AFogManager::FindActorPos()
{
	TArray<AActor*> Characters; //��������. �ƿ����̳ʿ� �ִ� '��� ����'�� �迭 �ڡڡ��ʿ��� ������ �� �𸣰ڴ�
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), Characters); //ã���� �ϴ� Ŭ������ ����� ���͸� ��� �� ã���ش�

	TArray<AActor*> Weapons;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeapon::StaticClass(), Weapons);

	//ã�Ƽ� ��̿� ������ ��°� �ƴ϶� characterSpwan�� �ɾ���� ��������Ʈ�� �迭�� �����ϰ�ʹ�.

	for (int32 i = 0; i < Weapons.Num(); i++)
	{
		Characters.Add(Weapons[i]);
	}

	for (int32 i = 0; i < Characters.Num(); i++)
	{
		actorPos = Characters[i]->GetActorLocation(); //ĳ����(����)���� ��ġ
		actorPos.Z = GetActorLocation().Z; //ĳ����(����)�� ���̿� AliveZone ���̸� �����ش�. ĳ���ʹ� 0 ���� ���� �ְ� AliveZone�� ���̰� 0 �̾���.
		moveDist = FVector::Distance(this->GetActorLocation(), actorPos); //AlvieZone ����(��ġ)�� ���͵�(��ġ)�� �Ÿ�. ����

		if (moveDist > aliveRange) //����(�÷��̾�, ���ʹ� ��)�� Alive ���� �ȿ� ������, �� �ۿ� ������
		{
			DestroyObject(Characters[i]);
			//isAlive = false; //Alive �ƴ� ---> Kill
		}
		else //���Ͱ� Alive ���� �ȿ� �ִ�
		{
			//isAlive = true; //Alive ����
		}
	}

	
}


void AFogManager::DestroyObject(AActor* actor) //�ڡڡ� �߰�. ų�� �ʿ� ��� �� �� �ϴ�
{
	ARIM_Player* Player = Cast<ARIM_Player>(actor);
	if (Player != nullptr) // ų���� ���� ���� �÷��̾��� ��
	{
		if (Player->playerState != EPlayerState::Die)
		{
			Player->Die(); //ų���� �÷��̾ ���̸� �״� �ڵ�
		}
	}
	ASH_Enemy* Enemy = Cast<ASH_Enemy>(actor);
	if (Enemy != nullptr)
	{
		if (Enemy->fsm->mState != EEnemyState::Die)
		{
			Enemy->fsm->stateChangeMontage(EEnemyState::Die, TEXT("Die"));
		}
	}
	AWeapon* weapon = Cast<AWeapon>(actor);
	if (weapon != nullptr)
	{
		AItemSpawn* ItemSpawn = Cast<AItemSpawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AItemSpawn::StaticClass()));
		ItemSpawn->DeleteWeapon(weapon);
		weapon->Destroy();
	}
}







