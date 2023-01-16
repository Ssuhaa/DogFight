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
	//SpringArm ������Ʈ ���� ����
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* compSpringArm;
	
	//Camera ������Ʈ ���� ����
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* compPlayerCam;

public:
	//�ȱ� �ӵ�
	UPROPERTY(EditAnywhere)
	float walkSpeed = 800; //���ʿ� �� ��ġ ����

	//�޸��� �ӵ� 
	UPROPERTY(EditAnywhere)
	float runSpeed = 1500; //������ �ʿ�� ����

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

	//���ű/������ �̺�Ʈ ó�� �Լ�
	void InputKickToss();

	//��ġ�� �̺�Ʈ ó�� �Լ�
	void InputHeadbutt();

	//���� ������ �̺�Ʈ ó�� �Լ�
	void InputDropWeapon();

	//���� ��� Ű�� �Լ��� Weapon ���� ó���ߴ�

// 	//���� ��� �̺�Ʈ ó�� �Լ�
// 	bool isinWeaponRange = false;

public:
	//[�� ����ƽ�޽� ������Ʈ �߰�]
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMeshGun;

	//[�Ѹ��� ����ƽ�޽� ������Ʈ �߰�]
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMeshLollipop;

	//[�Ѹ��� �浹 ������Ʈ(�浹ü) �߰�]
	UPROPERTY(EditAnywhere)
	class USphereComponent* compCollisionLollipop;

public:
	//[�Ѿ� ����]
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARIM_Bullet> bulletFactory;

public:
// 	//�÷��̾ ���� �� ���̸� false, ���̸� true
// 	bool isPlayerVisibleGun = false;
// 
// 	//�÷��̾ �Ѹ����� �� ���̸� false, ���̸� true
// 	bool isPlayerVisibleLollipop = false;

	//[�÷��̾ ���� ���̰� �ϴ� �Լ� ����]
 	UFUNCTION()
 	void VisibleGun();

	//[�÷����̾ �Ѹ����� ���̰� �ϴ� �Լ� ����]
	UFUNCTION()
	void VisibleLollipop();

// 	//[��, �Ѹ��� ���� ??? �Լ� ����]
// 	virtual void EnableInput(class APlayerController* PlayerController) override; //�ڡڡ� �� �𸣰���...
// 
// 	void GetWeapon();

public:
// 	//[�÷��̾� ������ ��ġ �浹 ������Ʈ ���� �߰�]
// 	UPROPERTY(EditAnywhere)
// 	class UBoxComponent* compCollisionPunchR;
// 
// 	//[�÷��̾� �޼� ��ġ �浹 ������Ʈ ���� �߰�]
// 	UPROPERTY(EditAnywhere)
// 	class UBoxComponent* compCollisionPunchL;
// 
// 	//[�÷��̾� ű �浹 ������Ʈ ���� �߰�] ���ӿ��� �޹߷� ű �Ѵ�
// 	UPROPERTY(EditAnywhere)
// 	class UBoxComponent* compCollisionKick;
// 
// 	//[�÷��̾� ��� �浹 ������Ʈ ���� �߰�]
// 	UPROPERTY(EditAnywhere) 
// 	class UBoxComponent* compCollisionHeadbutt;

public:
	//[��������Ʈ] �ڡڡ� �Ϻ��ϰ� �������� �� ��

	//[[2]�÷��̾� ��ġ �ݸ����� ���ʹ̰� �浹 �� ���� �� �Լ� ����]
	//��ġ �ݸ��� ������Ʈ ������ ��� �Լ� OnComponentBeginOverlap�� OnComponentEndOverlap ��������Ʈ ȣ��
// 	UFUNCTION()
// 	void collisionPunchRBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
// 	UFUNCTION()
// 	void collisionPunchLBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
// 	UFUNCTION()
// 	void collisionPunchREndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
// 	UFUNCTION()
// 	void collisionPunchLEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
// 
// 	//[[2]�÷��̾� ű �ݸ����� ���ʹ̰� �浹 �� ���� �� �Լ� ����]
// 	//ű �ݸ��� ������Ʈ ������ ��� �Լ� OnComponentBeginOverlap�� OnComponentEndOverlap ��������Ʈ ȣ��
// 	UFUNCTION()
// 	void collisionKickBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
// 	UFUNCTION()
// 	void collisionKickEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
// 
// 	//[[2]�÷��̾� ��ġ�� �ݸ����� ���ʹ̰� �浹 �� ���� �� �Լ� ����]
// 	UFUNCTION()
// 	void collisionHeadbuttBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
// 	UFUNCTION()
// 	void collisionHeadbuttEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
// 
// 	//[2]�÷��̾ �� �Ѹ��˰� ���ʹ̰� �浵�� �� ���� �� �Լ� ����]
// 	UFUNCTION()
// 	void collisonLollipopBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
// 	void collisonLollipopEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
// 	//[PunchGrab ��ư ����...?] 
// 	bool isInputPunchGrab = false; //�ڡڡ� �� �𸣰���. �� ������ �ұ�?
// 
// 	bool isinputLollipop = false;
// 
// 	//[KickToss ��ư ����...?]
// 	bool isInputKickToss = false; //�ڡڡ� �� �𸣰���. �� ������ �ұ�?
// 
// 	//[Headbutt ��ư ����...?]
// 	bool isInputHeadbutt = false; //�ڡڡ� �� �𸣰���. �� ������ �ұ�?
// 
// 	//[DropWeapon ��ư ����...?]
// 	bool isInputDropWeapon = false; //�ڡڡ� �� �𸣰���. �� ������ �ұ�?
// 
// 	//[�� ����...???]
// 	UPROPERTY()
// 	class AActor* currEnemy; //�ڡڡ� �Ϻ��ϰ� �������� �� ��

	//[���ʹ̰� �����ϸ� �÷��̾ �������� �޴� �Լ�] �÷��̾��� �������ϱ� �÷��̾�� ����
	UFUNCTION()
	void OnDamageProcess();

	//[�÷��̾� ����]
	UFUNCTION()
	void Die();
	
	//[�÷��̾� HP ����]
	int32 HP = 5;

	//[����ð�]
	float currentTime = 0;

	//[�÷��̾ ������ �� false, �������� �� true]
	bool isplayerDown = false; //����Ʈ�� �÷��̾� ���ִ�

	//[�÷��̾ �׾����� �� true, ������� �� false]
	bool isplayerDie = false; //����Ʈ�� �÷��̾� ����ִ�

public:
	//[�ִϸ��̼� ����...] �ڡڡ� �������� �𸣰ڴ�
	UPROPERTY()
	class URIM_PlayerAnim* animPlayer;

	//[�ִϸ��̼� ���� ��� ����]
	int32 rand;

public:
  	//[�ٴڿ� �� ���� �߰�]
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGunWeapon> weaponGun;

	//[�ٴڿ� �Ѹ��� ���� �߰�]
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ALollipopWeapon> weaponLollipop;

	//���ʹ�
	UPROPERTY(EditAnywhere)
	class ASH_Enemy* Enemy;

	//�ٴڿ� ������ ����
	class AWeapon* weapon;

	//��Ʈ�� Ÿ�� ������
	UFUNCTION()
	void TargetDotAttack();

	//���ʹ� ���
	UPROPERTY()
	TArray<class AActor*> enemyActor;
	UPROPERTY()
	TArray<class ASH_Enemy*> enemyarray;

};
