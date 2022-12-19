// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_EnemyFSM.h"
#include <SH_Player.h>
#include <SH_Enemy.h>
#include <Kismet/GameplayStatics.h>

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

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()); //���忡�� Ÿ�� ã�ƿ���
	target = Cast<ASH_Player>(actor); //�÷��̾�Ÿ������ ĳ����
	me = Cast<ASH_Enemy>(GetOwner()); //���� ��ü ��������

	// ...
	
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
	}

}
void USH_EnemyFSM::MoveState()//�̵� ���� �Լ�����
{
	FVector destination = target->GetActorLocation(); // �������� Ÿ���� ���� �����̼����� ����
	FVector dir = destination - me->GetActorLocation(); //Ÿ�� ����
	me->AddMovementInput(dir.GetSafeNormal()); //Ÿ�� �������� �̵�

	if (dir.Size() < attackRange) //���� Ÿ����� �Ÿ��� ���ݹ��� �ȿ� ������
	{
		mState = EEnemyState::Attack; // ���ݻ��·� ��ȯ
	}
}
void USH_EnemyFSM::AttackState()//���� ���� �Լ�����
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > attackDelayTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack")); //���ݷα�
		currentTime = 0; //����ð� �ʱ�ȭ
	}
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation()); //Ÿ����� �Ÿ� ���� ���
	if (distance > attackRange) // Ÿ����� �Ÿ��� ���ݹ������� �����
	{
		mState = EEnemyState::Move; //���ʹ� ���� ����� �̵�.
	}
}
void USH_EnemyFSM::DamageState() //�ǰ� ���� �Լ�����
{
	currentTime += GetWorld()->DeltaRealTimeSeconds;
	if (currentTime > damageDelayTime) //�ǰݴ��ð��� ������ �����·� ��ȯ
	{
		mState = EEnemyState::Idle;
		currentTime = 0;
	}
}
void USH_EnemyFSM::DieState() // ���� ���� �Լ�����
{

}

void  USH_EnemyFSM::OnDamageProcess() //�ǰݾ˸� �̺�Ʈ �Լ� ����
{

	if (hp > 0) //ü���� 0�̾ƴϸ� �ǰݻ��� ����
	{
		hp--;	//ü�� ����
		mState = EEnemyState::Damage;
	}
	else // ü���� 0�̸� �������·� ��ȯ
	{
		mState = EEnemyState::Die;
	}
	UE_LOG(LogTemp, Warning, TEXT("%d"), hp);
}