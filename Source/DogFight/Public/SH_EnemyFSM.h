// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SH_EnemyFSM.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FTargetinterfaceDel, class AActor*);

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


	UPROPERTY(VisibleAnywhere, Category=FSM)
	class AActor* target; // Ÿ�� ����
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<class AActor*> targets;//Ÿ�� ���� ���
	UPROPERTY(BlueprintReadOnly)
	class ASH_Enemy* enemy; // ���� �ƴ� ���ʹ�
	UPROPERTY(BlueprintReadOnly)
	class ARIM_Player* player; //�÷��̾� ����
	UPROPERTY(BlueprintReadOnly)
	class ASH_Enemy* me; //���� ���ʹ�

	UPROPERTY(EditAnywhere,Category=FSM)
	float attackRange = 0; // ���ݹ��� 
	float GunRange = 400.0f; //�ѵ���� �� �Ÿ�
	float LollipopRange = 120.0f; // �Ѹ��� ����� �� �Ÿ�
	float defaultRange = 150.0f; // �ƹ��͵� �����ʾ��� �� �Ÿ�
	float ShovelRange = 120.0f;
	float TennisRange = 120.0f;


	void OnDamageProcess(); //�ǰݾ˸�

	void RandomTarget(); // Ÿ�� ����
	FTargetinterfaceDel dieDelegate;
	void removeDieTarget(); // ���ʹ� �ڽ��� ������ �ٸ� ���ʹ� Ÿ�ٿ� �ڽ��� ����� �Լ�

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	int32 hp = 3; // ���ʹ� HP 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	int32 downCount  = 3; //�˹� ī��Ʈ
	int32 currDown = 0;

	UPROPERTY(BlueprintReadOnly)
	float currentTime = 0; //����ð�
	UPROPERTY(EditAnywhere, Category=FSM)
	float idleDalayTime = 0.5f; //��� ���ð�
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackDelayTime = 2.0f; //���� ���ð� 
	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 1.0f; //�ǰ� ���ð� 
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category=FSM)
	float downDelayTime = 10.0f; //�˹� ���ð� 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FSM)
	float WakeupDelayTime = 1.0f; //��� ���ð�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FSM)
	float PickupDelayTime = 0.2f; //��� ���ð�
	bool isDelay(float delaytime); //������ �Լ�


	UPROPERTY(BlueprintReadOnly)
	class UEnemyAnim* anim; //���ʹ� �ִϸ��̼�
	int32 randindex; //�ִϸ��̼� ��Ÿ�� �ε���
	void WeaponAnimChange(bool AttackPlay, float Range); //����ִ� ������ ���� �ִ� ��ȯ �Լ�

	void stateChange(EEnemyState State); //mState ����
	void stateChangeMontage(EEnemyState State, FString Name);//mstate ���� �� ��Ÿ�� ����
	void DropWeapon(); //���ʹ� ���� ����

	UFUNCTION(BlueprintCallable)
	void TargetDotAttack(); //���ʹ� ���� 
	bool isTargetinAttackRange(AActor* Target); // Ÿ�ٰ��� ���ݹ��� Ȯ��
	UPROPERTY(EditAnywhere)
	float traceRange = 200; // ���� Ÿ�ٰ��� �Ÿ�
	UPROPERTY(EditAnywhere)
	float EnemyAngle = 30;  // ���� ����

	FVector P; //�̵����� Dir

	UPROPERTY()
	TSubclassOf<class AGunWeapon> Gun; //������ �� ����
	UPROPERTY()
	TSubclassOf<class ALollipopWeapon> Lollipop; // ������ �Ѹ��� ����

	UPROPERTY()
	class AAIController* AI; //AI��Ʈ�ѷ�

	void SetDownUI();
	void SstDieUl();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDownWidget> downUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDownWidget> dieUI;

};