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
	//SpringArm 컴포넌트 변수 선언. USpringArmComponent 속성 추가
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* compSpringArm;
	
	//Camera 컴포넌트 변수 선언
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* compPlayerCam;

public:
	//걷기 속도
	UPROPERTY(EditAnywhere)
	float walkSpeed = 600; //★★★필요 시 수치 변경

	//달리기 속도 
	UPROPERTY(EditAnywhere)
	float runSpeed = 1200; //★★★추후 필요시 변경

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

	//무기 버리기 이벤트 처리 함수
	void InputDropWeapon();

	//드롭킥/던지기 이벤트 처리 함수
	//void InputKickToss();

	//박치기 이벤트 처리 함수
	//void InputHeadbutt();


public:
	//[총 스켈레탈메시 컴포넌트 멤버 변수 추가] --->스켈레탈 무기
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* compMeshGun;

	//[롤리팝 스태틱메시 컴포넌트 추가] ---->스태틱 무기
	//UPROPERTY(VisibleAnywhere)
	//class UStaticMeshComponent* compMeshLollipop;

public:
	//[BulletFactory 총알 공장 속성 추가]
	//총알 공장
	UPROPERTY(EditAnywhere, Category = BulletFactory) //★★★카테고리 필요 없으면 추후 삭제
	TSubclassOf<class ARIM_Bullet> bulletFactory; //★★★오류해결. 클래스 ABullet을 못찾는 거 같음 ---> 내 파일명 ARIM_Bullet 으로 해야 함

public:
	//[플레이어가 들고 있는 총이 안 보였다 보이는 함수 선언]
	UFUNCTION()
	void VisibleGun();

	//[플레어이어가 들고 있는 롤리팝이 안 보였다 보이는 함수 선언]
	//UFUNCTION()
	//void VisibleLollipop();

	//[???]
	virtual void EnableInput(class APlayerController* PlayerController) override;

public:
	//[플레이어 오른손 펀치 충돌 컴포넌트 변수 추가]
	UPROPERTY(EditAnywhere)
	class UBoxComponent* compCollisionPunchR;

	//[플레이어 왼손 펀치 충돌 컴포넌트 변수 추가]
	UPROPERTY(EditAnywhere)
	class UBoxComponent* compCollisionPunchL;

	//[플레이어 킥 충돌 컴포넌트 변수 추가] 게임에서 왼발로 킥 한다
	//class UBoxComponent* compCollisionKick;

	//[플레이어 헤딩 충돌 컴포넌트 변수 추가]
	//class UBoxComponent* compCollisionHeadbutt;

public:
	//[[2]플레이어 펀치 콜리전과 에너미가 충돌 시 실행 될 함수 선언]
	//펀치 콜리전 컴포넌트 변수의 멤버 함수 OnComponentBeginOverlap과 OnComponentEndOverlap 델리케이트 호출
	UFUNCTION()
	void collisionPunchRBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void collisionPunchLBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void collisionPunchREndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void collisionPunchLEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//[[2]플레이어	킥 콜리전과 에너미가 충돌 시 실행 될 함수 선언]
	//킥 콜리전 컴포넌트 변수의 멤버 함수 OnComponentBeginOverlap과 OnComponentEndOverlap 델리케이트 호출
// 	UFUNCTION()
// 	void collisionKickBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
// 	UFUNCTION()
// 	void collisionKickEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//[[2]플레이어 박치기 콜리전과 에너미가 충돌 시 실행 될 함수 선언]
// 	UFUNCTION()
// 	void collisionHeadbuttBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
// 	UFUNCTION()
// 	void collisionHeadbuttEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



public:
	//[???]
	bool isInputPunchGrab = false;

	//[???]
	//bool isInputKickToss = false;

	//[???]
	//bool isInputHeadbutt = false;

	//[???]
	UPROPERTY()
	class AActor* currEnemy;

	//[에너미가 공격하면 플레이어가 데미지를 받는 함수] 플레이어의 데미지니까 플레이어에서 구현
	UFUNCTION()
	void OnDamageProcess();
	
	//[플레이어 풀 HP 변수]
	int32 HP = 5;

	//[현재시간]
	float currentTime = 0;

	UPROPERTY()
	TSubclassOf<class ARIM_Gun> Gun;

	

};
