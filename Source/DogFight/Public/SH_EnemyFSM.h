// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SH_EnemyFSM.generated.h"

UENUM(BlueprintType) //��������
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
	Down,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOGFIGHT_API USH_EnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USH_EnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM) 
	EEnemyState mState = EEnemyState::Idle; //���º��� mState

	void IdleState();//��� ���� �Լ�����
	void MoveState();//�̵� ���� �Լ�����
	void AttackState();	//���� ���� �Լ�����
	void DamageState(); //�ǰ� ���� �Լ�����
	void DieState(); // ���� ���� �Լ�����
	void DownState();//�˹� ���� �Լ�


	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float idleDalayTime = 2; //���ð�
	UPROPERTY(BlueprintReadOnly)
	float currentTime = 0; //����ð�

	UPROPERTY(VisibleAnywhere, Category=FSM)
	class ACharacter* target; // �÷��̾� Ÿ�� ���� ����

	UPROPERTY()
	TArray<class AActor*> targets; //��� ���� �迵��

	UPROPERTY()
	class ASH_Enemy* enemy; // �����ƴ� ���ʹ�

	UPROPERTY()
	class ASH_Player* player; //�÷��̾�

	float dir = 1000.0f; // ���� Ÿ���� �Ÿ�

	UPROPERTY()
	class ASH_Enemy* me; //�������� ���� ����

	UPROPERTY(EditAnywhere,Category=FSM)
	float attackRange = 170.0f; // ���ݹ��� ���� ����

	UPROPERTY(EditAnywhere, Category=FSM)
	float attackDelayTime = 1.0f; //���ݴ��ð� ���� ����

	void OnDamageProcess(); //�ǰݾ˸��̺�Ʈ �Լ� ����

	void SeachShortTarget(); //����� Ÿ�� ã��
	void SeachLongTarget(); // �� Ÿ�� ã��

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	int32 hp = 5; // ���ʹ� HP ��������

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	int32 downCount  = 3; //�˹� ī��Ʈ

	int32 randindex; //�ִϸ��̼� ��Ÿ�� �ε���

	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 1.0f; //�ǰ� ���ð� ���� ����

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category=FSM)
	float dieDelayTime = 5.0f; //�˹� ���ð� ��������

	UPROPERTY()
	class UEnemyAnim* anim; //���ʹ� �ִϸ��̼�

	bool isAttackState = false;
};