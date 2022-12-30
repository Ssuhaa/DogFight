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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GunMesh) //�ڡڡ�ī�װ� �ʿ� ������ ���� ����
	class USkeletalMeshComponent* compMeshGun;

public:
	//[BulletFactory �Ѿ� ���� �Ӽ� �߰�]
	//�Ѿ� ����
	UPROPERTY(EditAnywhere, Category = BulletFactory) //�ڡڡ�ī�װ� �ʿ� ������ ���� ����
	TSubclassOf<class ARIM_Bullet> bulletFactory; //�ڡڡڿ����ذ�. Ŭ���� ABullet�� ��ã�� �� ���� ---> �� ���ϸ� ARIM_Bullet ���� �ؾ� ��

public:
	//[�÷��̾ ��� �ִ� ���� �� ������ ���̴� �Լ� ����]
	UFUNCTION()
	void VisibleGun();

	//[???]
	virtual void EnableInput(class APlayerController* PlayerController) override;

public:
	//[�÷��̾� ������ ��ġ �浹 ������Ʈ ���� �߰�]
	UPROPERTY(EditAnywhere)
	class UBoxComponent* compCollisionPunchR;

	//[�÷��̾� �޼� ��ġ �浹 ������Ʈ ���� �߰�]
	UPROPERTY(EditAnywhere)
	class UBoxComponent* compCollisionPunchL;

public:
	//[�÷��̾� �ָ԰� �浹 �� �Լ�]
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

	//[�÷��̾ �����ؼ� ���ʹ̰� �������� �޴� �Լ�]
	UFUNCTION()
	void OnDamageProcess();
	
	//[�÷��̾� HP ����]
	int32 HP = 10;


};
