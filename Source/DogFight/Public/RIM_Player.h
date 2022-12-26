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
	//SpringArm ������Ʈ ���� ����. USpringArmComponent �Ӽ� �߰�
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;
	
	//Camera ������Ʈ ���� ����
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* playerCamComp;



public:
	//�ȱ� �ӵ�
	UPROPERTY(EditAnywhere) //�ڡڡڿ��� ���� �� Category = PlayerSetting
	float walkSpeed = 200;

	//�޸��� �ӵ� 
	UPROPERTY(EditAnywhere) //�ڡڡڿ��� ���� �� Category = PlayerSetting
	float runSpeed = 600;

	//�̵� ����
	FVector direction;

public:

	//�¿� ȸ�� �Է� �̺�Ʈ ó�� �Լ�. 
	void Turn(float value);

	//�¿� �̵� �Է� �̺�Ʈ ó�� �Լ�. InputHorizontal �Լ� ����
	void InputHorizontal(float value);

    //���� �̵� �Է� �̺�Ʈ ó�� �Լ�. InputVertical �Լ� ����
	void InputVertical(float value);

	//�÷��̾� �̵� ó��
	void Move();

public:
	//���� �Է� �̺�Ʈ ó�� �Լ�. InputJump �Լ� ����
	void InputJump();

	//�޸��� �Է� �̺�Ʈ ó�� �Լ�. InputRun �Լ� ����
	void InputRun();

	//����/��� �̺�Ʈ ó�� �Լ�. �Ѿ� �߻� ó�� �Լ�. InputPunchGrab �Լ� ����
	void InputPunchGrab();

	//���ű/������ �̺�Ʈ ó�� �Լ�.
	void KickToss();

	//���� ������ �̺�Ʈ ó�� �Լ�.
	void DropWeapon();

	//��Ÿ�� �̺�Ʈ ó�� �Լ�.
	void Climb();

	//��ġ�� �̺�Ʈ ó�� �Լ�.
	void Headbutt();

	//������ �̺�Ʈ ó�� �Լ�.
	void Roll();

	//�޸��� ���� �̺�Ʈ ó�� �Լ�. �ڡڡڱ��� �� �ص� ��
    //�޸��� ���� �̺�Ʈ ó�� �Լ�. �ڡڡھƸ� ���� �� �ص� ��



public:
	//[�� ���̷�Ż�޽� ������Ʈ ��� ���� �߰�]
	//�� ���̷�Ż �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

public:
	//[BulletFactory �Ѿ� ���� �Ӽ� �߰�]
	//�Ѿ� ����
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ARIM_Bullet> bulletFactory; //�ڡڡڿ���. Ŭ���� ABullet�� ��ã�� �� ���� ---> �� ���ϸ� ARIM_Bullet ���� �ؾ� ��

public:


};
