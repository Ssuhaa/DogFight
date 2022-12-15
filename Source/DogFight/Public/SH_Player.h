// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SH_Player.generated.h"

UCLASS()
class DOGFIGHT_API ASH_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASH_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	class USpringArmComponent* springArmComp;//스프링암컴포넌트 변수 선언
	UPROPERTY(VisibleAnywhere, Category=Camera)
	class UCameraComponent* camComp;//카메라컴포넌트 변수 선언


	void Move(); // 플레이어 이동처리
	
	void Turn(float value);//좌우 회전입력처리
	void LookUp(float value); //상하 회전입력처리

	UPROPERTY(EditAnywhere, Category=PlayerSetting)
	float walkSpeed = 600; //이동속도
	FVector direction; //이동 방향

	void InputHorizontal(float value); // 좌우이동 이벤트 입력처리
	void InputVertical(float value);// 상하 이동 이벤트 입력처리
	void InputJump(); //점프 이벤트 입력처리


};
