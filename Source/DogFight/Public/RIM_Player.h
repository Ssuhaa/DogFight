// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RIM_Player.generated.h"

UENUM(BlueprintType)
enum class EPlayerState: uint8
{
	Idle,
	KnockDown,
	Die,
};

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
	//SpringArm ������Ʈ
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* compSpringArm;
	
	//Camera ������Ʈ
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
	//�¿� ȸ�� �Է� �Լ�
	void Turn(float value);

	//�¿� �̵� �Է� �Լ�
	void InputHorizontal(float value);

    //���� �̵� �Է� �Լ�
	void InputVertical(float value);

	//�÷��̾� �̵� �Լ�
	void Move();

public:
	//���� �Է� �Լ�
	void InputJump();

	//�޸��� �Է� �Լ�
	void InputRun();

	//��ġ, ��, �Ѹ��� �Է� �Լ�
	void InputPunchGrab();

	//ű �Է� �Լ�
	void InputKickToss();

	//��ġ�� �Է� �Լ�
	void InputHeadbutt();

	//���� ��� Ű�� �Լ��� Weapon ���� ����

	//���� ������ �Է� �Լ�
	void InputDropWeapon();

public:
	//[�÷��̾��� �� ����ƽ�޽� ������Ʈ �߰�]
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMeshGun;

	//[�÷��̾��� �Ѹ��� ����ƽ�޽� ������Ʈ �߰�]
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMeshLollipop;

	//[�÷��̾��� �Ѹ��� �浹 ������Ʈ(�浹ü) �߰�]
	UPROPERTY(EditAnywhere)
	class USphereComponent* compCollisionLollipop;

	//[�Ѿ� ���� ���]
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARIM_Bullet> bulletFactory;

public:
	//[�÷��̾ ���� ���̰� �ϴ� �Լ�]
 	UFUNCTION()
 	void VisibleGun();

	//[�÷����̾ �Ѹ����� ���̰� �ϴ� �Լ�]
	UFUNCTION()
	void VisibleLollipop();

public:
	//[���ʹ̰� �����ϸ� �÷��̾ �������� �޴� �Լ�] �÷��̾��� �������ϱ� �÷��̾�� ����
	UFUNCTION()
	void OnDamageProcess();

	//[�÷��̾� ���� �Լ�]
	UFUNCTION()
	void Die();
	
	//[�÷��̾� HP]
	int32 HP = 5;

	//[����ð�]
	float currentTime = 0;

// 	//[�÷��̾ ������ �� false, �������� �� true]
// 	bool isplayerDown = false; //����Ʈ�� �÷��̾� ���ִ�
// 
// 	//[�÷��̾ �׾����� �� true, ������� �� false]
// 	bool isplayerDie = false; //����Ʈ�� �÷��̾� ����ִ�


public:
	//[�ִϸ��̼� ����...] �ڡڡ� �������� �𸣰ڴ�
	UPROPERTY()
	class URIM_PlayerAnim* animPlayer;

	//[�ִϸ��̼� ���� ��� ����]
	int32 rand;

public:
  	//[�ٴڿ� �� ���� ���]
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGunWeapon> weaponGun;

	//[�ٴڿ� �Ѹ��� ���� ���]
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

	//enum. bool ��� ���
	EPlayerState playerState = EPlayerState::Idle;

};
