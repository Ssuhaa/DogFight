// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SH_EnemyFSM.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FTargetinterfaceDel, class AActor*);

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


	UPROPERTY(VisibleAnywhere, Category=FSM)
	class AActor* target; // 타겟 변수
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<class AActor*> targets;//타겟 변수 어레이
	UPROPERTY(BlueprintReadOnly)
	class ASH_Enemy* enemy; // 내가 아닌 에너미
	UPROPERTY(BlueprintReadOnly)
	class ARIM_Player* player; //플레이어 변수
	UPROPERTY(BlueprintReadOnly)
	class ASH_Enemy* me; //소유 에너미

	UPROPERTY(EditAnywhere,Category=FSM)
	float attackRange = 0; // 공격범위 
	float GunRange = 400.0f; //총들었을 때 거리
	float LollipopRange = 120.0f; // 롤리팝 들었을 때 거리
	float defaultRange = 150.0f; // 아무것도 들지않았을 때 거리
	float ShovelRange = 120.0f;
	float TennisRange = 120.0f;


	void OnDamageProcess(); //피격알림

	void RandomTarget(); // 타겟 설정
	FTargetinterfaceDel dieDelegate;
	void removeDieTarget(); // 에너미 자신이 죽으면 다른 에너미 타겟에 자신을 지우는 함수

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	int32 hp = 3; // 에너미 HP 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	int32 downCount  = 3; //넉백 카운트
	int32 currDown = 0;

	UPROPERTY(BlueprintReadOnly)
	float currentTime = 0; //경과시간
	UPROPERTY(EditAnywhere, Category=FSM)
	float idleDalayTime = 0.5f; //대기 대기시간
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackDelayTime = 2.0f; //공격 대기시간 
	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 1.0f; //피격 대기시간 
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category=FSM)
	float downDelayTime = 10.0f; //넉백 대기시간 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FSM)
	float WakeupDelayTime = 1.0f; //깨어남 대기시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FSM)
	float PickupDelayTime = 0.2f; //잡기 대기시간
	bool isDelay(float delaytime); //딜레이 함수


	UPROPERTY(BlueprintReadOnly)
	class UEnemyAnim* anim; //에너미 애니메이션
	int32 randindex; //애니메이션 몽타주 인덱스
	void WeaponAnimChange(bool AttackPlay, float Range); //들고있는 웨폰에 따른 애님 전환 함수

	void stateChange(EEnemyState State); //mState 변경
	void stateChangeMontage(EEnemyState State, FString Name);//mstate 변경 및 몽타주 변경
	void DropWeapon(); //에너미 무기 버림

	UFUNCTION(BlueprintCallable)
	void TargetDotAttack(); //에너미 공격 
	bool isTargetinAttackRange(AActor* Target); // 타겟과의 공격범위 확인
	UPROPERTY(EditAnywhere)
	float traceRange = 200; // 내적 타겟과의 거리
	UPROPERTY(EditAnywhere)
	float EnemyAngle = 30;  // 내적 각도

	FVector P; //이동상태 Dir

	UPROPERTY()
	TSubclassOf<class AGunWeapon> Gun; //스폰할 건 액터
	UPROPERTY()
	TSubclassOf<class ALollipopWeapon> Lollipop; // 스폰할 롤리팝 액터

	UPROPERTY()
	class AAIController* AI; //AI컨트롤러

	void SetDownUI();
	void SstDieUl();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDownWidget> downUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDownWidget> dieUI;

};