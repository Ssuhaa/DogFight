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
#include <Engine/SkeletalMesh.h>


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
	PlayerController->InputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, &AWeapon::BindGetWeapon);
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
	
	GetWeapon();
}

void AWeapon::GetWeapon()
{
	player = Cast<ARIM_Player>(overlapActor);
	if (player != nullptr)
	{	

		//���� �÷��̾��� Gun or Lollipop �޽ð� ���̸�(�÷��̾ Gun, Lollipop �� ��� �ִ�)
		//�÷��̾��� Gun �޽ÿ��� IsVisible �Լ�(�÷��̾��� Gun �޽� ���̰� �ϴ� �Լ�)�� true �̰ų�
		//�÷��̾��� LolliPop �޽ÿ��� IsVisible �Լ�(�÷��̾��� LolliPop �޽� ���̰� �ϴ� �Լ�)�� true �̸�
		//�Լ��� ������. �������� �ʴ´�
		if (player->compMeshGun->IsVisible() == true || player->compMeshLollipop->IsVisible() == true) return; 

			if (Soketname == TEXT("Gun")) //���� �̸��� �ؽ�Ʈ�� Gun �̸�
			{
				UE_LOG(LogTemp, Error, TEXT("Player Gun Pickup!")); //Ȯ�ο� �ؽ�Ʈ ���
					
 				//�÷��̾� �ִϸ��̼� ��Ÿ�� �� '�Ⱦ�' �ִϸ��̼� ��� �ڡڡڿ���. ���߿� �ϱ�
// 				URIM_PlayerAnim* animPlayer = Cast<URIM_PlayerAnim>(GetMesh()->GetAnimInstance());
// 				animPlayer->PlayPlayerAnim(TEXT("Pickup"), 0);

				player->VisibleGun(); //�÷��̾��� Gun �޽ð� ���̰� �Ѵ�. �÷��̾ Gun �� ��� �ְ� �Ѵ�. ---> �÷��̾� Gun ������ �Լ� ȣ��
				//player->isInputPunchGrab = false; //InputPunchGrab ��ư ���� �ʱ�ȭ(false) �ڡڡڻ����ص� �� ��
				Destroy(); //�ٴ��� Gun �� �ı��Ѵ�
			}
			else if (Soketname == TEXT("Lollipop")) //���� �̸��� �ؽ�Ʈ�� Lollipop �̸�
			{
				UE_LOG(LogTemp, Error, TEXT("Player Lollipop Pickup!")); //Ȯ�ο� �ؽ�Ʈ ���
					
 				//�÷��̾� �ִϸ��̼� ��Ÿ�� �� '�Ⱦ�' �ִϸ��̼� ��� �ڡڡڿ���. ���ֿ� �ϱ�
// 				URIM_PlayerAnim* animPlayer = Cast<URIM_PlayerAnim>(GetMesh()->GetAnimInstance());
// 				animPlayer->PlayPlayerAnim(TEXT("Pickup"), 0);

				player->VisibleLollipop(); //�÷��̾��� Lollipop �޽ð� ���̰� �Ѵ�. �÷��̾ Lollipop �� ��� �ְ� �Ѵ�. ---> �÷��̾� Lollipop ������ �Լ� ȣ��
				//player->isInputPunchGrab = false; //InputPunchGrab ��ư ���� �ʱ�ȭ(false) �ڡڡڻ����ص� �� ��
				Destroy(); //�ٴ��� Lollipop �� �ı��Ѵ�
			}
// 			else �ڡڡ�else �κ� �����ص� �� ��
// 			{
// 				player->isInputPunchGrab = false;
// 				return;
// 			}
	}

	Enemy = Cast<ASH_Enemy>(overlapActor);
	if (Enemy != nullptr)
	{
		if (Enemy->fsm->mState == EEnemyState::Damage || Enemy->fsm->mState == EEnemyState::Down || Enemy->fsm->mState == EEnemyState::Die|| Enemy->fsm->mState == EEnemyState::Attack) return;
		if(Enemy->fsm->anim->isGunget == false && Enemy->fsm->anim->isLollipopget == false)
		{
			Enemy->GetEnemyWeapon(compMesh->GetStaticMesh(), Soketname);
			Enemy->fsm->removeWeaponArray();
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
