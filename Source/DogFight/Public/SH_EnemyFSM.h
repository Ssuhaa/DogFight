// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SH_EnemyFSM.generated.h"

UENUM(BlueprintType) //상태정의
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
	EEnemyState mState = EEnemyState::Idle; //상태변수 mState

	void IdleState();//대기 상태 함수선언
	void MoveState();//이동 상태 함수선언
	void AttackState();	//공격 상태 함수선언
	void DamageState(); //피격 상태 함수선언
	void DieState(); // 죽음 상태 함수선언
	void DownState();//넉백 상태 함수
	void PickupState();//픽업 상태함수
	void WakeupState();// 일어나는 상태 함수


	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float idleDalayTime = 1; //대기시간
	UPROPERTY(BlueprintReadOnly)
	float currentTime = 0; //경과시간

	UPROPERTY(VisibleAnywhere, Category=FSM)
	class AActor* target; // 플레이어 타겟 변수 선언

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<class AActor*> targets; //모든 액터 배열 받을 변수

	UPROPERTY()
	TArray<class AActor*> Weaponarray;

	UPROPERTY(BlueprintReadOnly)
	class ASH_Enemy* enemy; // 내가아닌 에너미 변수

	UPROPERTY(BlueprintReadOnly)
	class ARIM_Player* player; //플레이어 변수

	UPROPERTY(BlueprintReadOnly)
	class AWeapon* weapon;

	UPROPERTY()
	class ASH_Enemy* me; //소유액터 변수 

	UPROPERTY(EditAnywhere,Category=FSM)
	float attackRange = 150.0f; // 공격범위 변수 
	float GunRange = 400.0f;
	float LollipopRange = 120.0f;
	float defaultRange = 150.0;

	UPROPERTY(EditAnywhere, Category=FSM)
	float attackDelayTime = 2.0f; //공격대기시간 변수

	void OnDamageProcess(); //피격알림이벤트 함수 
	void RandomTarget();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	int32 hp = 3; // 에너미 HP 변수선언

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	int32 downCount  = 3; //넉백 카운트

	int32 randindex; //애니메이션 몽타주 인덱스

	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 3.0f; //피격 대기시간 변수 선언

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category=FSM)
	float downDelayTime = 10.0f; //넉백 대기시간 변수선언

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FSM)
	float WakeupDelayTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FSM)
	float PickupDelayTime = 0.2f;

	UPROPERTY(BlueprintReadOnly)
	class UEnemyAnim* anim; //에너미 애니메이션

	bool isAttackState = false;

	void stateChange(EEnemyState State); //mState 변경
	void stateChangeMontage(EEnemyState State, FString Name);//mstate변경 및 몽타주 변경
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