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
	Pickup,
	Wakeup,
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
	void PickupState();//�Ⱦ� �����Լ�
	void WakeupState();// �Ͼ�� ���� �Լ�


	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float idleDalayTime = 1; //���ð�
	UPROPERTY(BlueprintReadOnly)
	float currentTime = 0; //����ð�

	UPROPERTY(VisibleAnywhere, Category=FSM)
	class AActor* target; // �÷��̾� Ÿ�� ���� ����

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<class AActor*> targets; //��� ���� �迭 ���� ����

	UPROPERTY()
	TArray<class AActor*> Weaponarray;

	UPROPERTY(BlueprintReadOnly)
	class ASH_Enemy* enemy; // �����ƴ� ���ʹ� ����

	UPROPERTY(BlueprintReadOnly)
	class ARIM_Player* player; //�÷��̾� ����

	UPROPERTY(BlueprintReadOnly)
	class AWeapon* weapon;

	UPROPERTY()
	class ASH_Enemy* me; //�������� ���� 

	UPROPERTY(EditAnywhere,Category=FSM)
	float attackRange = 150.0f; // ���ݹ��� ���� 
	float GunRange = 400.0f;
	float LollipopRange = 120.0f;
	float defaultRange = 150.0;

	UPROPERTY(EditAnywhere, Category=FSM)
	float attackDelayTime = 2.0f; //���ݴ��ð� ����

	void OnDamageProcess(); //�ǰݾ˸��̺�Ʈ �Լ� 
	void RandomTarget();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	int32 hp = 3; // ���ʹ� HP ��������

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	int32 downCount  = 3; //�˹� ī��Ʈ

	int32 randindex; //�ִϸ��̼� ��Ÿ�� �ε���

	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 3.0f; //�ǰ� ���ð� ���� ����

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category=FSM)
	float downDelayTime = 10.0f; //�˹� ���ð� ��������

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FSM)
	float WakeupDelayTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FSM)
	float PickupDelayTime = 0.2f;

	UPROPERTY(BlueprintReadOnly)
	class UEnemyAnim* anim; //���ʹ� �ִϸ��̼�

	bool isAttackState = false;

	void stateChange(EEnemyState State); //mState ����
	void stateChangeMontage(EEnemyState State, FString Name);//mstate���� �� ��Ÿ�� ����
	void addTargetarray();
	void removeWeaponArray();
	void addWeaponArray();
	void DropWeapon();

	UFUNCTION(BlueprintCallable)
	void TargetDotAttack();

	UPROPERTY(EditAnywhere)
	float traceRange = 200;
	UPROPERTY(EditAnywhere)
	float EnemyAngle = 30;


	bool isDelay(float delaytime);

	FVector P;

	UPROPERTY()
	TSubclassOf<class AGunWeapon> Gun;

	UPROPERTY()
	TSubclassOf<class ALollipopWeapon> Lollipop;

	void removeDieTarget();

	UPROPERTY()
	class AAIController* AI;

	bool bplayerAttack = false;

	void WeaponAnimChange(bool bAttackPlay, float Range);
};