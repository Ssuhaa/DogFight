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
	class USpringArmComponent* springArmComp;//��������������Ʈ ���� ����
	UPROPERTY(VisibleAnywhere, Category=Camera)
	class UCameraComponent* camComp;//ī�޶�������Ʈ ���� ����


	void Move(); // �÷��̾� �̵�ó��
	
	void Turn(float value);//�¿� ȸ���Է�ó��
	void LookUp(float value); //���� ȸ���Է�ó��

	UPROPERTY(EditAnywhere, Category=PlayerSetting)
	float walkSpeed = 600; //�̵��ӵ�
	FVector direction; //�̵� ����

	void InputHorizontal(float value); // �¿��̵� �̺�Ʈ �Է�ó��
	void InputVertical(float value);// ���� �̵� �̺�Ʈ �Է�ó��
	void InputJump(); //���� �̺�Ʈ �Է�ó��


};
