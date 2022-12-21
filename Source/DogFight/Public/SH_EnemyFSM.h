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

	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float idleDalayTime = 2; //대기시간
	float currentTime = 0; //경과시간

	UPROPERTY(VisibleAnywhere, Category=FSM)
	class ASH_Player* target; // 플레이어 타겟 변수 선언

	UPROPERTY()
	class ASH_Enemy* me; //소유액터 변수 선언

	UPROPERTY(EditAnywhere,Category=FSM)
	float attackRange = 150.0f; // 공격범위 변수 선언

	UPROPERTY(EditAnywhere, Category=FSM)
	float attackDelayTime = 2.0f; //공격대기시간 변수 선언

	void OnDamageProcess(); //피격알림이벤트 함수 선언

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	int32 hp = 3; // 에너미 HP 변수선언

	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 2.0f; //피격 대기시간 변수 선언

	UPROPERTY(EditAnywhere, Category=FSM)
	float dieDelayTime = 10.0f; //넉백 대기시간 변수선언

	UPROPERTY()
	class UEnemyAnim* anim;
};