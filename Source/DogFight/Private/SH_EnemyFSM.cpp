// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_EnemyFSM.h"
#include <SH_Player.h>
#include <SH_Enemy.h>
#include <Kismet/GameplayStatics.h>
#include <EnemyAnim.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetArrayLibrary.h>

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

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), targets); //��� ���� �迭�� ��������
	me = Cast<ASH_Enemy>(GetOwner()); //���� ��ü ��������
	SeachShortTarget();

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
}

void USH_EnemyFSM::IdleState()//��� ���� �Լ�����
{
	currentTime += GetWorld()->DeltaTimeSeconds; //�ð��帧 ����.
	if (currentTime > idleDalayTime) //���� ����ð��� ���ð��� �ʰ��ߴٸ�
	{
		isAttackState = false;
		stateChange(EEnemyState::Move);
		currentTime = 0; //����ð� �ʱ�ȭ
	}
}

void USH_EnemyFSM::MoveState()//�̵� ���� �Լ�����
{
	// �������� Ÿ���� ���� �����̼����� ����
	FVector P = target->GetActorLocation() - me->GetActorLocation(); //Ÿ�� ����
	me->AddMovementInput(P.GetSafeNormal()); //Ÿ�� �������� �̵�
	if (P.Size() < attackRange) //���� Ÿ����� �Ÿ��� ���ݹ��� �ȿ� ������
	{
		me->SetActorRotation(UKismetMathLibrary::MakeRotFromXZ(P, FVector::UpVector));// Ÿ�ٹ����� �ٶ󺸰�
		stateChange(EEnemyState::Attack);
		anim->bAttackPlay = true;
		currentTime = attackDelayTime;
	}
	
}

void USH_EnemyFSM::AttackState()//���� ���� �Լ�����
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > attackDelayTime)
	{
		isAttackState = true;
		float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation()); //Ÿ����� �Ÿ� ���� ���
		if (distance > attackRange) // Ÿ����� �Ÿ��� ���ݹ������� �����
		{
			isAttackState = false;
			stateChange(EEnemyState::Move);
			currentTime = 0;
		}
		anim->bAttackPlay = true;
		stateChange(EEnemyState::Idle);
		currentTime = 0; //����ð� �ʱ�ȭ
	}

}

void USH_EnemyFSM::DamageState() //�ǰ� ���� �Լ�����
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > damageDelayTime) //�ǰݴ��ð��� ������ �����·� ��ȯ
	{
		anim->Montage_Stop(damageDelayTime);
		stateChange(EEnemyState::Attack);
		currentTime = 0;

	}
}

void USH_EnemyFSM::DownState() //�˹� ���� �Լ� ����
{
	currentTime += GetWorld()->DeltaRealTimeSeconds;
	if (currentTime > dieDelayTime)
	{
		anim->Montage_Stop(damageDelayTime);
		SeachLongTarget();
		stateChange(EEnemyState::Idle);
		currentTime = 0;
		downCount--;
		hp = 5;
	}
}

void USH_EnemyFSM::DieState() // ���� ���� �Լ� ����.
{


}

void  USH_EnemyFSM::OnDamageProcess() //�ǰݾ˸� �̺�Ʈ �Լ� ����
{

	if (hp > 0 && downCount > 0) //ü���� 0�̾ƴϰ� �ٿ�ī��Ʈ�� 0�̾ƴϸ� �ǰݻ��� ����
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy HP : %d"), hp);
		randindex = FMath::RandRange(0, 1);
		stateChangeMontage(EEnemyState::Damage, "Damage");
		currentTime = 0;
		hp--;
	}
	else if (hp < 1 && downCount > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Down : %d"), downCount);
		stateChangeMontage(EEnemyState::Down, "Down");

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy DIE"));
		stateChangeMontage(EEnemyState::Die, "Down");
	}

}

void USH_EnemyFSM::SeachShortTarget() //����� Ÿ�� ã��
{

	for (int32 i = 0; i < targets.Num(); i++)
	{
		if (targets[i] != me)
		{
			float distance = FVector::Distance(targets[i]->GetActorLocation(), me->GetActorLocation());
			if (distance < dir)
			{
				dir = distance;
				target = Cast<ACharacter>(targets[i]);

			}
		}
	}
}//���� �����Ÿ�� ã��

void USH_EnemyFSM::SeachLongTarget() // �� Ÿ�� ã��
{
	for (int32 i = 0; i < targets.Num(); i++)
	{
		if (targets[i] != me)
		{
			float distance = FVector::Distance(targets[i]->GetActorLocation(), me->GetActorLocation());
			if (distance > dir)
			{
				dir = distance;
				target = Cast<ACharacter>(targets[i]);

			}
		}
	}
} // ���� �� Ÿ�� ã��

void USH_EnemyFSM::stateChange(EEnemyState state)//������Ʈ ���� �� �ִϸ��̼� ����ȭ
{
	mState = state;
	anim->animState = mState;
} 

void USH_EnemyFSM::stateChangeMontage(EEnemyState State, char* Name)
{
	FString sectionName = FString::Printf(TEXT("%s%d"), Name, randindex);
	mState = State;
	anim->PlayDamagaAnim(FName(*sectionName));
	anim->animState = mState;

} 