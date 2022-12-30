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

	//좌우 회전 입력 이벤트 처리 함수. 
	void Turn(float value);

	//좌우 이동 입력 이벤트 처리 함수. InputHorizontal 함수 선언
	void InputHorizontal(float value);

    //상하 이동 입력 이벤트 처리 함수. InputVertical 함수 선언
	void InputVertical(float value);

	//플레이어 이동 처리
	void Move();

public:
	//점프 입력 이벤트 처리 함수. InputJump 함수 선언
	void InputJump();

	//달리기 입력 이벤트 처리 함수. InputRun 함수 선언
	void InputRun();

	//공격/잡기 이벤트 처리 함수. 총알 발사 처리 함수. InputPunchGrab 함수 선언
	void InputPunchGrab();

	//드롭킥/던지기 이벤트 처리 함수.
	void KickToss();

	//무기 버리기 이벤트 처리 함수.
	void DropWeapon();

	//벽타기 이벤트 처리 함수.
	void Climb();

	//박치기 이벤트 처리 함수.
	void Headbutt();

	//구르기 이벤트 처리 함수.
	void Roll();

	//달리기 점프 이벤트 처리 함수. ★★★구현 안 해도 됨
    //달리기 공격 이벤트 처리 함수. ★★★아마 구현 안 해도 됨



public:
	//[총 스켈레탈메시 컴포넌트 멤버 변수 추가]
	//총 스켈레탈 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GunMesh) //★★★카테고리 필요 없으면 추후 삭제
	class USkeletalMeshComponent* compMeshGun;

public:
	//[BulletFactory 총알 공장 속성 추가]
	//총알 공장
	UPROPERTY(EditAnywhere, Category = BulletFactory) //★★★카테고리 필요 없으면 추후 삭제
	TSubclassOf<class ARIM_Bullet> bulletFactory; //★★★오류해결. 클래스 ABullet을 못찾는 거 같음 ---> 내 파일명 ARIM_Bullet 으로 해야 함

public:
	//[플레이어가 들고 있는 총이 안 보였다 보이는 함수 선언]
	UFUNCTION()
	void VisibleGun();

	//[???]
	virtual void EnableInput(class APlayerController* PlayerController) override;

public:
	//[플레이어 오른손 펀치 충돌 컴포넌트 변수 추가]
	UPROPERTY(EditAnywhere)
	class UBoxComponent* compCollisionPunchR;

	//[플레이어 왼손 펀치 충돌 컴포넌트 변수 추가]
	UPROPERTY(EditAnywhere)
	class UBoxComponent* compCollisionPunchL;

public:
	//[플레이어 주먹과 충돌 시 함수]
	UFUNCTION()
	void collisionPunchRBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void collisionPunchLBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void collisionPunchREndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void collisionPunchLEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//[???]
	bool isInputPunchGrab = false;

	//[???]
	UPROPERTY()
	class AActor* currEnemy;

	//[플레이어가 공격해서 에너미가 데미지를 받는 함수]
	UFUNCTION()
	void OnDamageProcess();
	
	//[플레이어 HP 변수]
	int32 HP = 10;


};
