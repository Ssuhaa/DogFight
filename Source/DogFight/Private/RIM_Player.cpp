// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Player.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "RIM_Bullet.h" //�ڡڡڿ����ذ�. Bullet.h ���� �ҽ��� �������ٰ� �Ѵ� ---> �� ���ϸ� RIM_Bullet.h ���� �ؾ� ��

// Sets default values
ARIM_Player::ARIM_Player() //������
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[���̷�Ż�޽�. �� �߰�]
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor2.Charactor2'")); //�ڡڡ��ӽ÷� ĳ����2 ����. ���� �ʿ�� ����
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object); 
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
		//gunMeshComp->SetVisibility(false); //�ڡڡ�Ȯ�� �ʿ�. ����Ʈ�� �� �� ���̰� �ϴ� �ڵ� �̰� �ƴѰ�??? �������� �Ǵµ�, ���� ����. ���� �ȵ�
	}



	//[�������� ������Ʈ �߰�/USpringArmComponent �߰�]
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90)); //�ڡڡ����� �ʿ�� ����
	springArmComp->TargetArmLength = 400; //�ڡڡ����� �ʿ�� ����
	springArmComp->bUsePawnControlRotation = true; //�Է¿� ���� ȸ�� ����

	//[ī�޶� ������Ʈ �߰�/UCameraComponent �߰�]
	playerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("playerCamComp"));
	playerCamComp->SetupAttachment(springArmComp);
	playerCamComp->bUsePawnControlRotation = false; //�Է¿� ���� ȸ�� ����

	bUseControllerRotationYaw = true; //�Է¿� ���� ȸ�� ����



	//[�� ���̷�Ż�޽� ������Ʈ ���] //�ڡڡ�å�� ���̷�Ż�޽÷� ���� �����ϰ� �ߴµ�, ���ε� �� ���̷�Ż�޽÷� �ؾ��ϴ��� �ñ�
	//�� ���̷�Ż�޽� ������Ʈ ���
	//�θ� ������Ʈ�� Mesh ������Ʈ�� ����
	//���̷�Ż�޽� ������ �ε�
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh(),TEXT("Gun")); //���� ĳ���� ���Ͽ� ����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	//[�� ���̷�Ż�޽� ������Ʈ ������ ����]
	//���̷�Ż�޽� ������ �ε尡 �����ߴٸ�
	//���̷�Ż�޽� ������ �Ҵ�
	//��ġ �����ϱ�
	if (TempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(-90, 0, -159)); //�ڡڡ��� ��ġ. �ӽ÷� ����. ���� ���� �ʿ�
		gunMeshComp->SetRelativeScale3D(FVector(0.5f)); //�ڡڡ��� ũ��. �ӽ÷� ����. ���� ���� �ʿ�
	}

}

// Called when the game starts or when spawned
void ARIM_Player::BeginPlay()
{
	Super::BeginPlay();

	//[�ʱ� �ӵ��� �ȱ�� ����]
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	gunMeshComp->SetVisibility(false);
	
}

// Called every frame
void ARIM_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//[Move(�÷��̾� �̵�) �Լ� ȣ��]
	Move();



}

//[Move(�÷��̾� �̵�) �Լ� ����?]
void ARIM_Player::Move()
{
	//[�÷��̾� �̵� ���� �ڵ�]

	//P(��� ��ġ) = P0(���� ��ġ) + v(�ӵ�=ũ��*����) * t(�ð�)
	//direction = FTransform(GetControlRotation()). TransformVector(direction);
	//FVector P0 = GetActorLocation();
	//FVector vt = walkSpeed * direction * DeltaTime; //v=walkSpeed * direction, t=DeltaTime
	//FVector P = P0 + vt;
	//SetActorLocation(P);
	//direction = FVector::ZeroVector; //���� direction�� ��� ���(x, y, z)�� 0�� �״��Ͽ� �ʱ�ȭ

	//AddMovementInput() �Լ��� �̿��Ͽ� ���� �ִ� ��ӿ �ڵ� ��ü
	direction = FTransform(GetControlRotation()).TransformVector(direction); //�̵� ������ ��Ʈ�� ���� �������� ��ȯ
	AddMovementInput(direction); //
	direction = FVector::ZeroVector;
}


// Called to bind functionality to input
void ARIM_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//[�¿� ȸ�� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ARIM_Player::Turn);

	//[�¿� �Է� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ARIM_Player::InputHorizontal);

	//[���� �Է� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ARIM_Player::InputVertical);

	//[���� �Է� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ARIM_Player::InputJump);

	//[�޸��� �Է� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &ARIM_Player::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &ARIM_Player::InputRun);

	//[����/��� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��] = �Ѿ� �߻�
	PlayerInputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, &ARIM_Player::InputPunchGrab);

	//[���ű/������ �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]

	//[���� ������ �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]
	
	//[��Ÿ�� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]
	
	//[��ġ�� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]

	//[������ �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]

	//[�޸��� ���� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��] �ڡڡڱ��� �� �ص� ��
	//[�޸��� ���� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��] �ڡڡھƸ� ���� �� �ص� ��

}

//[�¿� ȸ�� �Լ� ����]
void ARIM_Player::Turn(float value)
{
	AddControllerYawInput(value);
}

//[�¿� �Է� �Լ� ����]
void ARIM_Player::InputHorizontal(float value)
{
	direction.Y = value;
}

//[���� �Է� �Լ� ����]
void ARIM_Player::InputVertical(float value)
{
	direction.X = value;
}

//[���� �Է� �Լ� ����]
void ARIM_Player::InputJump()
{
	Jump();
}

//[�޸��� �Է� �Լ� ����]
void ARIM_Player::InputRun()
{
	auto movement = GetCharacterMovement();

	if (movement->MaxWalkSpeed > walkSpeed) //���� �޸��� �����(�ʱⰪ�� �ȱ� ����̹Ƿ� walkSeed���� ũ�� ����)
	{
		movement->MaxWalkSpeed = walkSpeed; //�ȱ� �ӵ��� ��ȯ
	}
	else
	{
		movement->MaxWalkSpeed = runSpeed; //�޸��� �ӵ��� ��ȯ
	}
}


//[����/��� �̺�Ʈ ó�� �Լ� ����] = �Ѿ� �߻�
void ARIM_Player::InputPunchGrab()
{
	//FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition")); //�ڡڡ�å �����ε� ���� �ʿ� ���� �� ����
	//GetWorld()->SpawnActor<ARIM_Bullet>(bulletFactory, firePosition); //�ڡڡ�å �����ε� ���� �ʿ� ���� �� ����
	if (gunMeshComp->IsVisible() == true)
	{
	GetWorld()->SpawnActor<ARIM_Bullet>(bulletFactory, GetActorLocation(), GetActorRotation());
	UE_LOG(LogTemp, Warning, TEXT("Attack!")); //�ڡڡڳ��߿� ����

	}
}



//[���ű/������ �̺�Ʈ ó�� �Լ� ����]

//[���� ������ �̺�Ʈ ó�� �Լ� ����]

//[��Ÿ�� �̺�Ʈ ó�� �Լ� ����]

//[��ġ�� �̺�Ʈ ó�� �Լ� ����]

//[������ �̺�Ʈ ó�� �Լ� ����]


//[���� �޸��� �̺�Ʈ ó�� �Լ� ����] �ڡڡڱ��� �� �ص� ��
//[�޸��� ���� �̺�Ʈ ó�� �Լ� ����] �ڡڡھƸ� ���� �� �ص� ��


//[�÷��̾ ��� �ִ� ���� �� ������ ���̴� �Լ� ����]
void ARIM_Player::VisibleGun()
{
	gunMeshComp->SetVisibility(true);
}



void ARIM_Player::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(LogTemp , Warning, TEXT("4444444444"));
}