// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RIM_Player.generated.h"

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
	//SpringArm 컴포넌트 변수 선언
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* compSpringArm;
	
	//Camera 컴포넌트 변수 선언
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
	//좌우 회전 입력 이벤트 처리 함수
	void Turn(float value);

	//좌우 이동 입력 이벤트 처리 함수
	void InputHorizontal(float value);

    //상하 이동 입력 이벤트 처리 함수
	void InputVertical(float value);

	//플레이어 이동 처리 함수
	void Move();

public:
	//점프 입력 이벤트 처리 함수
	void InputJump();

	//달리기 입력 이벤트 처리 함수
	void InputRun();

	//공격/잡기 이벤트 처리 함수
	void InputPunchGrab();

	//드롭킥/던지기 이벤트 처리 함수
	void InputKickToss();

	//박치기 이벤트 처리 함수
	void InputHeadbutt();

	//무기 버리기 이벤트 처리 함수
	void InputDropWeapon();

	//무기 들기 키와 함수는 Weapon 에서 처리했다

// 	//무기 들기 이벤트 처리 함수
// 	bool isinWeaponRange = false;

public:
	//[총 스태틱메시 컴포넌트 추가]
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMeshGun;

	//[롤리팝 스태틱메시 컴포넌트 추가]
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMeshLollipop;

	//[롤리팝 충돌 컴포넌트(충돌체) 추가]
	UPROPERTY(EditAnywhere)
	class USphereComponent* compCollisionLollipop;

public:
	//[총알 공장]
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARIM_Bullet> bulletFactory;

public:
// 	//플레이어에 총이 안 보이면 false, 보이면 true
// 	bool isPlayerVisibleGun = false;
// 
// 	//플레이어에 롤리팝이 안 보이면 false, 보이면 true
// 	bool isPlayerVisibleLollipop = false;

	//[플레이어에 총이 보이게 하는 함수 선언]
 	UFUNCTION()
 	void VisibleGun();

	//[플레어이어에 롤리팝이 보이게 하는 함수 선언]
	UFUNCTION()
	void VisibleLollipop();

// 	//[총, 롤리팝 관련 ??? 함수 선언]
// 	virtual void EnableInput(class APlayerController* PlayerController) override; //★★★ 잘 모르겠음...
// 
// 	void GetWeapon();

public:
// 	//[플레이어 오른손 펀치 충돌 컴포넌트 변수 추가]
// 	UPROPERTY(EditAnywhere)
// 	class UBoxComponent* compCollisionPunchR;
// 
// 	//[플레이어 왼손 펀치 충돌 컴포넌트 변수 추가]
// 	UPROPERTY(EditAnywhere)
// 	class UBoxComponent* compCollisionPunchL;
// 
// 	//[플레이어 킥 충돌 컴포넌트 변수 추가] 게임에서 왼발로 킥 한다
// 	UPROPERTY(EditAnywhere)
// 	class UBoxComponent* compCollisionKick;
// 
// 	//[플레이어 헤딩 충돌 컴포넌트 변수 추가]
// 	UPROPERTY(EditAnywhere) 
// 	class UBoxComponent* compCollisionHeadbutt;

public:
	//[델리케이트] ★★★ 완벽하게 이해하지 못 함

	//[[2]플레이어 펀치 콜리전과 에너미가 충돌 시 실행 될 함수 선언]
	//펀치 콜리전 컴포넌트 변수의 멤버 함수 OnComponentBeginOverlap과 OnComponentEndOverlap 델리케이트 호출
// 	UFUNCTION()
// 	void collisionPunchRBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
// 	UFUNCTION()
// 	void collisionPunchLBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
// 	UFUNCTION()
// 	void collisionPunchREndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
// 	UFUNCTION()
// 	void collisionPunchLEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
// 
// 	//[[2]플레이어 킥 콜리전과 에너미가 충돌 시 실행 될 함수 선언]
// 	//킥 콜리전 컴포넌트 변수의 멤버 함수 OnComponentBeginOverlap과 OnComponentEndOverlap 델리케이트 호출
// 	UFUNCTION()
// 	void collisionKickBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
// 	UFUNCTION()
// 	void collisionKickEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
// 
// 	//[[2]플레이어 박치기 콜리전과 에너미가 충돌 시 실행 될 함수 선언]
// 	UFUNCTION()
// 	void collisionHeadbuttBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
// 	UFUNCTION()
// 	void collisionHeadbuttEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
// 
// 	//[2]플레이어가 든 롤리팝과 에너미가 충도로 시 실행 될 함수 선언]
// 	UFUNCTION()
// 	void collisonLollipopBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
// 	void collisonLollipopEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
// 	//[PunchGrab 버튼 관련...?] 
// 	bool isInputPunchGrab = false; //★★★ 잘 모르겠음. 왜 만들어야 할까?
// 
// 	bool isinputLollipop = false;
// 
// 	//[KickToss 버튼 관련...?]
// 	bool isInputKickToss = false; //★★★ 잘 모르겠음. 왜 만들어야 할까?
// 
// 	//[Headbutt 버튼 관련...?]
// 	bool isInputHeadbutt = false; //★★★ 잘 모르겠음. 왜 만들어야 할까?
// 
// 	//[DropWeapon 버튼 관려...?]
// 	bool isInputDropWeapon = false; //★★★ 잘 모르겠음. 왜 만들어야 할까?
// 
// 	//[적 관련...???]
// 	UPROPERTY()
// 	class AActor* currEnemy; //★★★ 완벽하게 이해하지 못 함

	//[에너미가 공격하면 플레이어가 데미지를 받는 함수] 플레이어의 데미지니까 플레이어에서 구현
	UFUNCTION()
	void OnDamageProcess();

	//[플레이어 죽음]
	UFUNCTION()
	void Die();
	
	//[플레이어 HP 변수]
	int32 HP = 5;

	//[현재시간]
	float currentTime = 0;

	//[플레이어가 서있을 때 false, 누워있을 때 true]
	bool isplayerDown = false; //디폴트로 플레이어 서있다

	//[플레이어가 죽어있을 때 true, 살아있을 때 false]
	bool isplayerDie = false; //디폴트로 플레이어 살아있다

public:
	//[애니메이션 관련...] ★★★ 무엇인지 모르겠다
	UPROPERTY()
	class URIM_PlayerAnim* animPlayer;

	//[애니메이션 랜덤 재생 변수]
	int32 rand;

public:
  	//[바닥에 총 생성 추가]
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGunWeapon> weaponGun;

	//[바닥에 롤리팝 생성 추가]
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

};
