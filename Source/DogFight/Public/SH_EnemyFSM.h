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

	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float idleDalayTime = 2; //���ð�
	float currentTime = 0; //����ð�

	UPROPERTY(VisibleAnywhere, Category=FSM)
	class ASH_Player* target; // �÷��̾� Ÿ�� ���� ����

	UPROPERTY()
	class ASH_Enemy* me; //�������� ���� ����

	UPROPERTY(EditAnywhere,Category=FSM)
	float attackRange = 150.0f; // ���ݹ��� ���� ����

	UPROPERTY(EditAnywhere, Category=FSM)
	float attackDelayTime = 2.0f; //���ݴ��ð� ���� ����

	void OnDamageProcess(); //�ǰݾ˸��̺�Ʈ �Լ� ����

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	int32 hp = 3; // ���ʹ� HP ��������

	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 2.0f; //�ǰ� ���ð� ���� ����

	UPROPERTY(EditAnywhere, Category=FSM)
	float dieDelayTime = 10.0f; //�˹� ���ð� ��������

	UPROPERTY()
	class UEnemyAnim* anim;
};