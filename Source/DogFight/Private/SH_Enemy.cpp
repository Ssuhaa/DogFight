// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Enemy.h"
#include "SH_EnemyFSM.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/Character.h>
#include <Components/BoxComponent.h>
#include "SH_Player.h"


// Sets default values
ASH_Enemy::ASH_Enemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor1.Charactor1'")); //�޽� ������ ã��
	if (TempMesh.Succeeded())//���� �޽������͸� ã�Ҵٸ�
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object); // ���ʹ� �޽������� ����
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0)); // ���ʹ� ��ġ�� ȸ���� ����
	}

	fsm = CreateDefaultSubobject<USH_EnemyFSM>(TEXT("FSM")); //	���ʹ̿� FSM������Ʈ �߰�


	GetCharacterMovement()->bOrientRotationToMovement = true;
	// �ִϸ��̼� �������Ʈ �Ҵ�
	ConstructorHelpers::FClassFinder<UAnimInstance> tempclass(TEXT("AnimBlueprint'/Game/BluePrint/ABP_EnemyAnim.ABP_EnemyAnim_C'")); //���� C�� ����� ������ ��������
	if (tempclass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempclass.Class);
	}
	//���� �ݸ���
	compAttack = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	compAttack->SetupAttachment(GetMesh(), TEXT("Prop_Socket"));
	compAttack->SetBoxExtent(FVector(25));

	//!!!!!!!!���ʹ� �ݸ��� �����Ұ�.

}

// Called when the game starts or when spawned
void ASH_Enemy::BeginPlay()
{
	Super::BeginPlay();
	compAttack->OnComponentBeginOverlap.AddDynamic(this, &ASH_Enemy::attackBoxBeginOverlap); //������Ʈ �������� ��������Ʈ ���ε�
	compAttack->OnComponentEndOverlap.AddDynamic(this, &ASH_Enemy::attackBoxEndOverlap);
}

// Called every frame
void ASH_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void ASH_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ASH_Enemy::attackBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != fsm->me)
	{
		if (OtherActor->GetName().Contains(TEXT("Enemy")))
		{
			currEnemy = Cast<ASH_Enemy>(OtherActor);
			if (fsm->isAttackState == true)
			{
				if (fsm->mState == EEnemyState::Down || fsm->mState == EEnemyState::Die || fsm->mState == EEnemyState::Damage) return;
				if (currEnemy->fsm->mState != EEnemyState::Down && currEnemy->fsm->mState != EEnemyState::Die && currEnemy->fsm->mState != EEnemyState::Damage)
				{
					currEnemy->fsm->OnDamageProcess();
				}
				else
				{
					fsm->SeachLongTarget();
				}


			}

		}
		else if (OtherActor->GetName().Contains(TEXT("Player")))
		{
			//!!!!!!!!if �÷��̾ �¾Ҵ��� ���� �Ǵ�. ������ ���¸�
			//!!!!!!!!�ð� ���� �� ������ ���°� �ʿ���.
			player = Cast<ASH_Player>(OtherActor);
			player->OnDamageProcess();
		}
	}
}


void ASH_Enemy::attackBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}