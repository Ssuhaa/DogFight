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

	//점프 달리기 이벤트 처리 함수.

	//공격/잡기 이벤트 처리 함수. 총알/화살 발사 처리 함수. InputPunchGrab 함수 선언
	void InputPunchGrab(); //★★★오류.

	//달리기 공격 이벤트 처리 함수.

	//벽타기 이벤트 처리 함수.

	//드롭킥/던지기 이벤트 처리 함수.
	
	//박치기 이벤트 처리 함수.

	//구르기 이벤트 처리 함수.

	//무기 버리기 이벤트 처리 함수.

public:
	//[총 스켈레탈메시 컴포넌트 멤버 변수 추가]
	//총 스켈레탈 메시
	UPROPERTY(VisibleAnywhere, Category=GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

	//[활 스켈레탈메시 컴포넌트 멤버 변수 추가]
	//활 스켈레탈 메시
	UPROPERTY(VisibleAnywhere, Category=BowMesh)
	class USkeletalMeshComponent* bowMeshComp;

	//[삽 스켈레탈메시 컴포넌트 멤버 변수 추가]
	//삽 스켈레탈 메시
	UPROPERTY(VisibleAnywhere, Category = ShovelMesh)
	class USkeletalMeshComponent* shovelMeshComp;

	//[야구망방이 스켈레탈메시 컴포넌트 멤버 변수 추가]
	//야구망방이 스켈레탈 메시
	UPROPERTY(VisibleAnywhere, Category = BaseballbatMesh)
	class USkeletalMeshComponent* baseballbatMeshComp;

public:
	//[BulletFactory 총알 공장 속성 추가]
	//총알 공장
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ARIM_Bullet> bulletFactory; //★★★오류. 클래스 ABullet을 못찾는 거 같음.

	//[ArrowFactory 화살 공장 속성 추가]
	//화살 공장
	UPROPERTY(EditDefaultsOnly, Category = ArrowFactory)
	TSubclassOf<class ARIM_Arrow> arrowFactory; //★★★오류. 클래스 AArrow을 못찾는 거 같음

};
