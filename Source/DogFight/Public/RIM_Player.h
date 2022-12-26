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
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;
	
	//Camera 컴포넌트 변수 선언
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* playerCamComp;



public:
	//걷기 속도
	UPROPERTY(EditAnywhere) //★★★오류 나서 뺌 Category = PlayerSetting
	float walkSpeed = 200;

	//달리기 속도 
	UPROPERTY(EditAnywhere) //★★★오류 나서 뺌 Category = PlayerSetting
	float runSpeed = 600;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

public:
	//[BulletFactory 총알 공장 속성 추가]
	//총알 공장
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ARIM_Bullet> bulletFactory; //★★★오류. 클래스 ABullet을 못찾는 거 같음 ---> 내 파일명 ARIM_Bullet 으로 해야 함

public:


};
