// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_EnemyFSM.h"
#include <SH_Player.h>
#include <SH_Enemy.h>
#include <Kismet/GameplayStatics.h>
#include <EnemyAnim.h>

// Sets default values for this component's properties
USH_EnemyFSM::USH_EnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USH_EnemyFSM::BeginPlay()
{
	Super::BeginPlay();

// 	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()); //���忡�� Ÿ�� ã�ƿ���
// 	target = Cast<ASH_Player>(actor); //�÷��̾�Ÿ������ ĳ����
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), targets); //��� ���� �迭�� ��������
	me = Cast<ASH_Enemy>(GetOwner()); //���� ��ü ��������
	for (int32 i = 0; i < targets.Num(); i++) // �ݺ��� ������
	{
		if (targets[i] != me) //���� Ÿ���� ���� �ƴϰ�
		{
			float distance = FVector::Distance(targets[i]->GetActorLocation(), me->GetActorLocation()); //Ÿ�ٰ��� �Ÿ�
			if (distance < dir) //Ÿ���� �Ÿ��� Dir���� ������
			{
				dir = distance; //dir�� Ÿ���� �Ÿ� ����
				target = Cast<ACharacter>(targets[i]); // Ÿ�ٿ� ĳ���ͷ� ����ȯ�ؼ� ����.
			}
		}
	}



	// UEnemyAnim�Ҵ�
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	
}


// Called every frame
void USH_EnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	switch (mState) // ĳ���� ���� ������ ���� ����Ī ���� �ش� �Լ� ȣ���ϴ� �ڵ�
	{
	case EEnemyState::Idle: 
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case  EEnemyState::Down:
		DownState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
	// ...
}


void USH_EnemyFSM::IdleState()//��� ���� �Լ�����
{
	currentTime += GetWorld()->DeltaTimeSeconds; //�ð��帧 ����.
	if (currentTime > idleDalayTime) //���� ����ð��� ���ð��� �ʰ��ߴٸ�
	{
		mState = EEnemyState::Move; // �̵����·� ��ȯ
		currentTime = 0; //����ð� �ʱ�ȭ
		anim->animState = mState;
	}

}
void USH_EnemyFSM::MoveState()//�̵� ���� �Լ�����
{
	 // �������� Ÿ���� ���� �����̼����� ����
	FVector P = target->GetActorLocation() - me->GetActorLocation(); //Ÿ�� ����
	me->AddMovementInput(P.GetSafeNormal()); //Ÿ�� �������� �̵�

	if (P.Size() < attackRange) //���� Ÿ����� �Ÿ��� ���ݹ��� �ȿ� ������
	{
		mState = EEnemyState::Attack; // ���ݻ��·� ��ȯ
		anim->animState = mState;
		anim->bAttackPlay = true;
		currentTime = attackDelayTime;
	}
}
void USH_EnemyFSM::AttackState()//���� ���� �Լ�����
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > attackDelayTime)
	{
		if (target->GetName().Contains(TEXT("Enemy"))) ///���� Ÿ���� �̸��� ���ʹ̸� �����ϸ�
		{
			enemy = Cast<ASH_Enemy>(target); //���ʹ̿� Ÿ���� ����ȯ
			if (enemy->fsm->mState != EEnemyState::Damage && enemy->fsm->mState != EEnemyState::Die && enemy->fsm->mState != EEnemyState::Down) //
			{
				if(enemy->fsm->hp <0)
				{
					enemy->fsm->OnDamageProcess();
					anim->bAttackPlay = true;
				}
			}
			else
			{
				for (int32 i = 0; i < targets.Num(); i++) //��ü Ÿ�ٿ��� �ݺ���
				{
					if (targets[i] != me && targets[i] != enemy) //Ÿ���� �����ƴϰ� ���� ���� ���ʹ̰� �ƴϸ�
					{
						float distance = FVector::Distance(targets[i]->GetActorLocation(), me->GetActorLocation()); //�Ÿ� ���
						if (distance < dir)
						{
							dir = distance;
							target = Cast<ACharacter>(targets[i]); // Ÿ�� �� ����
						}
					}
				}
				mState = EEnemyState::Move; 
				anim->animState = mState;
			}
				currentTime = 0; //����ð� �ʱ�ȭ
		}
		else if(target->GetName().Contains(TEXT("Player")))
		{
			UE_LOG(LogTemp,Warning,TEXT("Enemy attack Player!!"));
			currentTime = 0;
		}
		
	}
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation()); //Ÿ����� �Ÿ� ���� ���
	if (distance > attackRange) // Ÿ����� �Ÿ��� ���ݹ������� �����
	{
		mState = EEnemyState::Move; //���ʹ� ���� ����� �̵�.
		anim->animState = mState;
	}
}
void USH_EnemyFSM::DamageState() //�ǰ� ���� �Լ�����
{
	currentTime += GetWorld()->DeltaRealTimeSeconds;
	if (currentTime > damageDelayTime) //�ǰݴ��ð��� ������ �����·� ��ȯ
	{
		mState = EEnemyState::Idle;
		currentTime = 0;
		anim->animState = mState;
	}
}
void USH_EnemyFSM::DownState() //�˹� ���� �Լ� ����
{
	
	currentTime = 0;
	currentTime += GetWorld()->DeltaRealTimeSeconds;
	if (downCount > 0)
	{
		if (anim->bDieDone == false)
		{
			if (currentTime > dieDelayTime)
			{
				UE_LOG(LogTemp, Warning, TEXT("Enemy Fall down!"));
				anim->Montage_Stop(damageDelayTime);
				mState = EEnemyState::Idle; // ��� ������Ʈ ��ȯ
				anim->animState = mState;
				currentTime = 0;
				hp = 5;
				downCount--;
				UE_LOG(LogTemp, Warning, TEXT("%d"), downCount);

			}

		}
	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT("Enemy DIE"));
		mState = EEnemyState::Die;
		currentTime = 0;
		anim->animState = mState;
		downCount = 0;
	}

}
void USH_EnemyFSM::DieState() // ���� ���� �Լ� ����.
{

}

void  USH_EnemyFSM::OnDamageProcess() //�ǰݾ˸� �̺�Ʈ �Լ� ����
{
	currentTime = 0;
	currentTime += GetWorld()->DeltaRealTimeSeconds;
 	if (hp > 0 && downCount>0) //ü���� 0�̾ƴϸ� �ǰݻ��� ����
 	{
		hp--; //HP ���̳ʽ�
		UE_LOG(LogTemp, Warning, TEXT("HP : %d"), hp);
		if(currentTime<dieDelayTime)
		{ 
 			mState = EEnemyState::Damage;
 			currentTime = 0;
 			int32 index = FMath::RandRange(0, 1);
 			FString sectionName = FString::Printf(TEXT("Damage%d"), index);
 			anim->PlayDamagaAnim(FName(*sectionName));
		}
 
 	
 	}
 	else // ü���� 0�̸� �������·� ��ȯ
 	{

		if (currentTime < dieDelayTime)
		{
 		mState = EEnemyState::Down;
		anim->PlayDamagaAnim((TEXT("die")));
		}
 	}
 	anim->animState = mState;

}