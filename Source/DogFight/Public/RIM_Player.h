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
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* compSpringArm;
	
	//Camera ������Ʈ ���� ����
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* compPlayerCam;

public:
	//�ȱ� �ӵ�
	UPROPERTY(EditAnywhere)
	float walkSpeed = 600; //�ڡڡ��ʿ� �� ��ġ ����

	//�޸��� �ӵ� 
	UPROPERTY(EditAnywhere)
	float runSpeed = 1200; //�ڡڡ����� �ʿ�� ����

	//�̵� ����
	FVector direction;

public:
	//�¿� ȸ�� �Է� �̺�Ʈ ó�� �Լ�
	void Turn(float value);

	//�¿� �̵� �Է� �̺�Ʈ ó�� �Լ�
	void InputHorizontal(float value);

    //���� �̵� �Է� �̺�Ʈ ó�� �Լ�
	void InputVertical(float value);

	//�÷��̾� �̵� ó�� �Լ�
	void Move();

public:
	//���� �Է� �̺�Ʈ ó�� �Լ�
	void InputJump();

	//�޸��� �Է� �̺�Ʈ ó�� �Լ�
	void InputRun();

	//����/��� �̺�Ʈ ó�� �Լ�
	void InputPunchGrab();

	//���� ������ �̺�Ʈ ó�� �Լ�
	void InputDropWeapon();

	//���ű/������ �̺�Ʈ ó�� �Լ�
	//void InputKickToss();

	//��ġ�� �̺�Ʈ ó�� �Լ�
	//void InputHeadbutt();


public:
	//[�� ���̷�Ż�޽� ������Ʈ ��� ���� �߰�] --->���̷�Ż ����
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* compMeshGun;

	//[�Ѹ��� ����ƽ�޽� ������Ʈ �߰�] ---->����ƽ ����
	//UPROPERTY(VisibleAnywhere)
	//class UStaticMeshComponent* compMeshLollipop;

public:
	//[BulletFactory �Ѿ� ���� �Ӽ� �߰�]
	//�Ѿ� ����
	UPROPERTY(EditAnywhere, Category = BulletFactory) //�ڡڡ�ī�װ� �ʿ� ������ ���� ����
	TSubclassOf<class ARIM_Bullet> bulletFactory; //�ڡڡڿ����ذ�. Ŭ���� ABullet�� ��ã�� �� ���� ---> �� ���ϸ� ARIM_Bullet ���� �ؾ� ��

public:
	//[�÷��̾ ��� �ִ� ���� �� ������ ���̴� �Լ� ����]
	UFUNCTION()
	void VisibleGun();

	//[�÷����̾ ��� �ִ� �Ѹ����� �� ������ ���̴� �Լ� ����]
	//UFUNCTION()
	//void VisibleLollipop();

	//[???]
	virtual void EnableInput(class APlayerController* PlayerController) override;

public:
	//[�÷��̾� ������ ��ġ �浹 ������Ʈ ���� �߰�]
	UPROPERTY(EditAnywhere)
	class UBoxComponent* compCollisionPunchR;

	//[�÷��̾� �޼� ��ġ �浹 ������Ʈ ���� �߰�]
	UPROPERTY(EditAnywhere)
	class UBoxComponent* compCollisionPunchL;

	//[�÷��̾� ű �浹 ������Ʈ ���� �߰�] ���ӿ��� �޹߷� ű �Ѵ�
	//class UBoxComponent* compCollisionKick;

	//[�÷��̾� ��� �浹 ������Ʈ ���� �߰�]
	//class UBoxComponent* compCollisionHeadbutt;

public:
	//[[2]�÷��̾� ��ġ �ݸ����� ���ʹ̰� �浹 �� ���� �� �Լ� ����]
	//��ġ �ݸ��� ������Ʈ ������ ��� �Լ� OnComponentBeginOverlap�� OnComponentEndOverlap ��������Ʈ ȣ��
	UFUNCTION()
	void collisionPunchRBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void collisionPunchLBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void collisionPunchREndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void collisionPunchLEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//[[2]�÷��̾�	ű �ݸ����� ���ʹ̰� �浹 �� ���� �� �Լ� ����]
	//ű �ݸ��� ������Ʈ ������ ��� �Լ� OnComponentBeginOverlap�� OnComponentEndOverlap ��������Ʈ ȣ��
// 	UFUNCTION()
// 	void collisionKickBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
// 	UFUNCTION()
// 	void collisionKickEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//[[2]�÷��̾� ��ġ�� �ݸ����� ���ʹ̰� �浹 �� ���� �� �Լ� ����]
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

	//[���ʹ̰� �����ϸ� �÷��̾ �������� �޴� �Լ�] �÷��̾��� �������ϱ� �÷��̾�� ����
	UFUNCTION()
	void OnDamageProcess();
	
	//[�÷��̾� Ǯ HP ����]
	int32 HP = 5;

	//[����ð�]
	float currentTime = 0;

	UPROPERTY()
	TSubclassOf<class ARIM_Gun> Gun;

	

};
