// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Player.h"
#include "GameFramework/SpringArmComponent.h"
#include <Camera/CameraComponent.h>
#include "SH_EnemyFSM.h"
#include <Components/BoxComponent.h>
#include "SH_Enemy.h"


// Sets default values
ASH_Player::ASH_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//���̷�Ż �޽� ����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor2.Charactor2'")); //ĳ���� �޽� ��θ� ã�� TempMesh�� �����Ѵ�.
	if (TempMesh.Succeeded()) //���� TempMesh�� ã�Ҵٸ�
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object); // ĳ���Ϳ� �޽��� �ִ´�.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0)); //ĳ���� �޽��� ��ġ���� ȸ������ ���� �����Ѵ�.
	}
	//�������� ������Ʈ
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp")); //SpringArm ������Ʈ ����
	springArmComp->SetupAttachment(RootComponent); // ��Ʈ ������Ʈ�� �ڽ����� ���̱�
	springArmComp->SetRelativeLocation(FVector(0, 70, 90)); // ��ġ ����
	springArmComp->TargetArmLength = 400; // Ÿ��� ī�޶� ���̿� �浹�� ���� ��� Ÿ����� ���� �Ÿ�
	springArmComp->bUsePawnControlRotation = true; // ȸ������ ������ �������� ������Ʈ�� ȸ����ų�� ����
	//ī�޶� ������Ʈ
	camComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camComp->SetupAttachment(springArmComp); // �������� ������Ʈ�� ī�޶� �ڽ����� ����
	camComp->bUsePawnControlRotation = false; //ȸ������ ������  ī�޶� ȸ����ų�� ���� 
	
	bUseControllerRotationYaw = true; //������� �Է¿� ����  ĳ���͸� ȸ����ų�� ����

	JumpMaxCount = 2; //�÷��̾ 2�� ������ �����ϰ� �����
	compAttack = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	compAttack->SetupAttachment(GetMesh(), TEXT("Rod_Socket"));


	
}

// Called when the game starts or when spawned
void ASH_Player::BeginPlay()
{
	Super::BeginPlay();
	
	compAttack->OnComponentBeginOverlap.AddDynamic(this, &ASH_Player::attackBoxBeginOverlap);
	compAttack->OnComponentEndOverlap.AddDynamic(this, &ASH_Player::attackBoxEndOverlap);

	
}

// Called every frame
void ASH_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(); // �÷��̾� �̵� �Լ�

	
	
}

// Called to bind functionality to input
void ASH_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ASH_Player::Turn); //������Ʈ �Է� �� ���� Turn�� Turn �Լ� ���ε�
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ASH_Player::LookUp); // ������Ʈ �Է� �� ���� LockUp�� LockUp �Լ� ���ε�
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ASH_Player::InputHorizontal); //������Ʈ �Է� ����� Horizontal�� InputHorizontal �Լ� ���ε�
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ASH_Player::InputVertical); // ������Ʈ �Է� �� ���� Vertical�� InputVertical �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ASH_Player::InputJump); //������Ʈ �Է� �� ���� Jump�� Jump �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, & ASH_Player::inputAttack); //������Ʈ �Է��� ���� Attack�� Attack �Լ� ���ε�
}

void ASH_Player::Move()
{
	//�÷��̾� �̵�����
	direction = FTransform(GetControlRotation()).TransformVector(direction);//�̵������� ��Ʈ�ѹ��� �������� ��ȯ . �� �ڵ尡 ������ ������� ���� 
	/*
	FVector P0 = GetActorLocation(); //������ġ
	FVector vt = direction * walkSpeed * DeltaTime; //�������ϴ� ��ġ * �̵��ӵ� * �ð�
	FVector P = P0 + vt; //�̷���ġ
	SetActorLocation(P); // �÷��̾� ��ġ ����
	*///�� �ڵ�� AddMovementInput() �Լ��� ��ü ����
	AddMovementInput(direction);
	direction = FVector::ZeroVector; // direction �ʱ�ȭ
}

void ASH_Player::Turn(float value)//�¿� ȸ���Է�ó�� �Լ�
{
	AddControllerYawInput(value);
}
void ASH_Player::LookUp(float value) //���� ȸ���Է�ó�� �Լ�
{
	AddControllerPitchInput(value);
}

void ASH_Player::InputHorizontal(float value)// �¿��̵� �̺�Ʈ �Է�ó�� �Լ�
{
	direction.Y = value;
}
void ASH_Player::InputVertical(float value)// ���� �̵� �̺�Ʈ �Է�ó�� �Լ�
{
	direction.X = value;
}
void ASH_Player::InputJump() //���� �̺�Ʈ �Է�ó��
{
	Jump();
}

void ASH_Player::inputAttack() //���� �̺�Ʈ �Է�ó��
{
	if (isinputAttack == true)
	{
		ASH_Enemy* enemy = Cast<ASH_Enemy>(currEenemy);
		enemy->fsm->OnDamageProcess();
	}
	
}

void ASH_Player::attackBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor->GetName().Contains(TEXT("Enemy")))
	{
		currEenemy = OtherActor;
		isinputAttack = true;
		
	}


}
void ASH_Player::attackBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isinputAttack = false;
}