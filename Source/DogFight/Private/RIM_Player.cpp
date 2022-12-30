// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Player.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Components/BoxComponent.h>
#include "RIM_Bullet.h"
#include "SH_Enemy.h"
#include "SH_EnemyFSM.h"

// Sets default values
ARIM_Player::ARIM_Player() //������
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[�÷��̾� ���̷�Ż�޽�. �� �߰�]
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor2.Charactor2'")); //�ڡڡ��ӽ÷� ĳ����2 ����. ���� �ʿ�� ����
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object); 
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	//[�������� ������Ʈ �߰�/USpringArmComponent �߰�]
	compSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	compSpringArm->SetupAttachment(RootComponent);
	compSpringArm->SetRelativeLocation(FVector(0, 70, 90)); //�ڡڡ����� �ʿ�� ����
	compSpringArm->TargetArmLength = 400; //�ڡڡ����� �ʿ�� ����
	compSpringArm->bUsePawnControlRotation = true; //�Է¿� ���� ȸ�� ����

	//[ī�޶� ������Ʈ �߰�/UCameraComponent �߰�]
	compPlayerCam = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamComp"));
	compPlayerCam->SetupAttachment(compSpringArm);
	compPlayerCam->bUsePawnControlRotation = false; //�Է¿� ���� ȸ�� ����

	bUseControllerRotationYaw = true; //�Է¿� ���� ȸ�� ����


	//[�� ���̷�Ż�޽� ������Ʈ ���]
	//���̷�Ż�޽� ������ �ε�
	//�θ� ������Ʈ�� Mesh ������Ʈ�� ����
	compMeshGun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	compMeshGun->SetupAttachment(GetMesh(), TEXT("Gun")); //���� ĳ���� ���Ͽ� ����
	
	//[�� ���̷�Ż�޽�. �� �߰�]
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (TempGunMesh.Succeeded())
	{
		compMeshGun->SetSkeletalMesh(TempGunMesh.Object);
		compMeshGun->SetRelativeLocation(FVector(5.759851f, -0.573986f, 26.993545f)); //�ڡڡ��� ��ġ. �ӽ÷� ����. ���� ���� �ʿ�
		compMeshGun->SetRelativeRotation(FRotator(0, -159, - 90)); //�ڡڡ��� ȸ��. �ӽ÷� ����. ���� ���� �ʿ�
		compMeshGun->SetRelativeScale3D(FVector(0.5f)); //�ڡڡ��� ũ��. �ӽ÷� ����. ���� ���� �ʿ�
	}


	//[�÷��̾� ��ġ �浹 ������Ʈ ���]
	//�浹 ������Ʈ �߰�
	//�浹 ������Ʈ�� ĳ���Ϳ� ����
	compCollisionPunchR = CreateDefaultSubobject<UBoxComponent>(TEXT("PunchMeshCompR"));
	compCollisionPunchL = CreateDefaultSubobject<UBoxComponent>(TEXT("PunchMeshCompL"));
	compCollisionPunchR->SetRelativeScale3D(FVector(0.5f));
	compCollisionPunchL->SetRelativeScale3D(FVector(0.5f));
	compCollisionPunchR->SetupAttachment(GetMesh(), TEXT("Prop_RSocket")); //������
	compCollisionPunchL->SetupAttachment(GetMesh(), TEXT("Prop_LSocket")); //�޼�
	
//�ҷ����丮�ȿ� �ҷ�����Ʈ�� �߰��Ѱ�
	ConstructorHelpers::FClassFinder<ARIM_Bullet> tempBul(TEXT("Blueprint'/Game/BluePrint/BP_RIM_Bullet.BP_RIM_Bullet_C'"));
	if(tempBul.Succeeded())
	{
		bulletFactory = tempBul.Class;
	}

}

// Called when the game starts or when spawned
void ARIM_Player::BeginPlay()
{
	Super::BeginPlay();

	//[�ʱ� �ӵ��� �ȱ�� ����]
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	
	//[���� ����Ʈ�� �Ⱥ��̰� ����]
	compMeshGun->SetVisibility(false);

	//[�÷��̾� �ָ԰� �浹 ��... ???]
	compCollisionPunchR->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionPunchRBeginOverlap);
	compCollisionPunchL->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionPunchLBeginOverlap);
	compCollisionPunchR->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionPunchREndOverlap);
	compCollisionPunchL->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionPunchLEndOverlap);


	
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

	//[����/��� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��] = �Ѿ� �߻� + ��ġ
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


//[����/��� �̺�Ʈ ó�� �Լ� ����] = �Ѿ� �߻� + ��ġ
void ARIM_Player::InputPunchGrab()
{	
	//���� �÷��̾ ���� ��� ������
	//���� �ѱ����� �Ѿ��� �߻��Ѵ�.
	if (compMeshGun->IsVisible() == true)
	{
		FTransform trFire = compMeshGun->GetSocketTransform(TEXT("FirPos")); //�� ���̷��濡�� ���� �߰�
		GetWorld()->SpawnActor<ARIM_Bullet>(bulletFactory, trFire); //���忡�� ARIM_Bullet(�Ѿ�) �����´�

		//GetWorld()->SpawnActor<ARIM_Bullet>(bulletFactory, GetActorLocation() + GetActorForwardVector() * 200, GetActorRotation());

	}
	else
	{
		if (isInputPunchGrab == true)
		{
			ASH_Enemy* Enemy = Cast<class ASH_Enemy>(currEnemy); //���ʹ� ����ȯ
			Enemy->fsm->OnDamageProcess(); //���ʹ�(���� ��� ����)���� fsm(���� ������Ʈ ��� ����)���� ���������μ��� ȣ��

			//��ġ �ִϸ��̼��� �÷����Ѵ�.

		}
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
	compMeshGun->SetVisibility(true);
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ARIM_Player::EnableInput(APlayerController* PlayerController)
{
	Super::EnableInput(PlayerController);

	//[����/��� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��] = �Ѿ� �߻�. Gun�� ���ε� �Ѱ�ٰ� �÷��̾ ������
	PlayerController->InputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, &ARIM_Player::InputPunchGrab);
}



//[�÷��̾� �ָ԰� �浹 �� �Լ� ����]
void ARIM_Player::collisionPunchRBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//���࿡ �ε��� OtherActor�� �̸��� enemy�� �����ϰ�������
	if (OtherActor->GetName().Contains(TEXT("Enemy")))  //GetDefaultSubojectByname
	{
		isInputPunchGrab = true;
		currEnemy = OtherActor;
	}
}

void ARIM_Player::collisionPunchLBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//���࿡ �ε��� OtherActor�� �̸��� enemy�� �����ϰ�������
	if (OtherActor->GetName().Contains(TEXT("Enemy")))
	{
		isInputPunchGrab = true;
		currEnemy = OtherActor;
	}
}

void ARIM_Player::collisionPunchREndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputPunchGrab = false;
}

void ARIM_Player::collisionPunchLEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputPunchGrab = false;
}



//[�÷��̾ ���� ���ϸ� ���ʹ̿��� ȣ��]
void ARIM_Player::OnDamageProcess()
{
	//HP ����
	//���� �÷��̾��� HP�� 0���� ���ų� ������ �÷��̾ �ı��Ѵ�
	HP--; 
	if (HP <= 0)
	{
		Destroy();
	}
}
