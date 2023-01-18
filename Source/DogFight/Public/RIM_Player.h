// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RIM_Player.generated.h"

UENUM(BlueprintType)
enum class EPlayerState: uint8
{
	Idle,
	KnockDown,
	Die,
};

UCLASS()
class DOGFIGHT_API ARIM_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARIM_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



public:
	//SpringArm 컴포넌트
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* compSpringArm;
	
	//Camera 컴포넌트
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* compPlayerCam;

public:
	//걷기 속도
	UPROPERTY(EditAnywhere)
	float walkSpeed = 800; //▶필요 시 수치 변경

	//달리기 속도 
	UPROPERTY(EditAnywhere)
	float runSpeed = 1500; //▶추후 필요시 변경

	//이동 방향
	FVector direction;

public:
	//좌우 회전 입력 함수
	void Turn(float value);

	//좌우 이동 입력 함수
	void InputHorizontal(float value);

    //상하 이동 입력 함수
	void InputVertical(float value);

	//플레이어 이동 함수
	void Move();

public:
	//점프 입력 함수
	void InputJump();

	//달리기 입력 함수
	void InputRun();

	//펀치, 총, 롤리팝 입력 함수
	void InputPunchGrab();

	//킥 입력 함수
	void InputKickToss();

	//박치기 입력 함수
	void InputHeadbutt();

	//무기 들기 키와 함수는 Weapon 에서 구현

	//무기 버리기 입력 함수
	void InputDropWeapon();

public:
	//[플레이어의 총 스태틱메시 컴포넌트 추가]
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMeshGun;

	//[플레이어의 롤리팝 스태틱메시 컴포넌트 추가]
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMeshLollipop;

	//[플레이어의 롤리팝 충돌 컴포넌트(충돌체) 추가]
	UPROPERTY(EditAnywhere)
	class USphereComponent* compCollisionLollipop;

	//[총알 공장 등록]
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARIM_Bullet> bulletFactory;

public:
	//[플레이어에 총이 보이게 하는 함수]
 	UFUNCTION()
 	void VisibleGun();

	//[플레어이어에 롤리팝이 보이게 하는 함수]
	UFUNCTION()
	void VisibleLollipop();

public:
	//[에너미가 공격하면 플레이어가 데미지를 받는 함수] 플레이어의 데미지니까 플레이어에서 구현
	UFUNCTION()
	void OnDamageProcess();

	//[플레이어 죽음 함수]
	UFUNCTION()
	void Die();
	
	//[플레이어 HP]
	int32 HP = 5;

	//[현재시간]
	float currentTime = 0;

// 	//[플레이어가 서있을 때 false, 누워있을 때 true]
// 	bool isplayerDown = false; //디폴트로 플레이어 서있다
// 
// 	//[플레이어가 죽어있을 때 true, 살아있을 때 false]
// 	bool isplayerDie = false; //디폴트로 플레이어 살아있다


public:
	//[애니메이션 관련...] ★★★ 무엇인지 모르겠다
	UPROPERTY()
	class URIM_PlayerAnim* animPlayer;

	//[애니메이션 랜덤 재생 변수]
	int32 rand;

public:
  	//[바닥에 총 생성 등록]
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGunWeapon> weaponGun;

	//[바닥에 롤리팝 생성 등록]
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ALollipopWeapon> weaponLollipop;

	//에너미
	UPROPERTY(EditAnywhere)
	class ASH_Enemy* Enemy;

	//바닥에 떨어진 무기
	class AWeapon* weapon;

	//도트로 타겟 때리기
	UFUNCTION()
	void TargetDotAttack();

	//에너미 어레이
	UPROPERTY()
	TArray<class AActor*> enemyActor;
	UPROPERTY()
	TArray<class ASH_Enemy*> enemyarray;

	//enum. bool 대신 사용
	EPlayerState playerState = EPlayerState::Idle;

};
