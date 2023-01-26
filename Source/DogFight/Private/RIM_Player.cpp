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
#include "RIM_Gun.h"
#include "RIM_PlayerAnim.h"
#include <Components/StaticMeshComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SphereComponent.h>
#include "GunWeapon.h"
#include "LollipopWeapon.h"
#include "CharacterSpawn.h"
#include "ItemSpawn.h"
#include "../DogFightGameModeBase.h"
#include <Kismet/KismetMathLibrary.h>
#include "ShovelWeapon.h"
#include "TennisWeapon.h"
#include "FogManager.h"

// Sets default values
ARIM_Player::ARIM_Player() //������
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[�÷��̾� ���̷�Ż�޽� ���. ����]
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor2.Charactor2'")); 
	if (tempMesh.Succeeded()) 
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object); 
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0)); 
	GetMesh()->SetCollisionProfileName(TEXT("Player")); //�ǣ��� ����ؼ� �־�� ��

	//[�÷��̾� ��ü �浹 ������Ʈ Collision ����]
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player")); //�ݸ��� �������� Player �� ����. Player ������ �������� ���� 

	//[�������� ������Ʈ �߰�. USpringArmComponent]
	compSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp")); 
	compSpringArm->SetupAttachment(RootComponent); //��Ʈ������Ʈ�� �ڽ����� ����
	compSpringArm->TargetArmLength = 900; //������ �ʿ�� ����
	compSpringArm->SocketOffset = FVector(0, 0, 450); //������ �ʿ�� ����
	compSpringArm->bUsePawnControlRotation = true; //�Է¿� ���� ȸ�� ����
	compSpringArm->bDoCollisionTest = false; //�������� ī�޶� �ݸ��� ��. ĳ���͵鳢�� ���� ���̴� ���� ����

	//[ī�޶� ������Ʈ �߰�. UCameraComponent]
	compPlayerCam = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamComp"));
	compPlayerCam->SetupAttachment(compSpringArm);
	compPlayerCam->SetRelativeLocation(FVector(0, 0, 1));
	compPlayerCam->SetRelativeRotation(FRotator(-25, 0, 0)); //������ �ʿ�� ����
	compPlayerCam->bUsePawnControlRotation = false; //�Է¿� ���� ȸ�� ����

	bUseControllerRotationYaw = false; //�Է¿� ���� ȸ�� ����. �̷��� �����ؾ��� ĳ���Ͱ� 360 ���ư���...
	GetCharacterMovement()->bOrientRotationToMovement = true; //������ �� �÷��̾� ����� �� ���̰� �� ���̰� �Ѵ�

	//[�� ����ƽ�޽� ������Ʈ �߰�]
	compMeshGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMeshComp"));
	compMeshGun->SetupAttachment(GetMesh(), TEXT("Gun")); //�θ� ������Ʈ�� Mesh ������Ʈ�� ����. ���� ĳ���� �� ���Ͽ� ����.
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempGunMesh(TEXT("StaticMesh'/Game/Geometry/Gun.Gun'"));
	if (tempGunMesh.Succeeded())
	{
		compMeshGun->SetStaticMesh(tempGunMesh.Object);
	}
	compMeshGun->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//[�Ѹ��� ����ƽ�޽� ������Ʈ �߰�]
	compMeshLollipop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LollipopMeshComp"));
	compMeshLollipop->SetupAttachment(GetMesh(), TEXT("Lollipop")); //�θ� ������Ʈ�� Mesh ������Ʈ�� ����. �Ѹ����� ĳ���� �� ���Ͽ� ����.
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempLollipopMesh(TEXT("StaticMesh'/Game/Geometry/Lollipop.Lollipop'"));
	if (tempLollipopMesh.Succeeded())
	{
		compMeshLollipop->SetStaticMesh(tempLollipopMesh.Object);
	}
	compMeshLollipop->SetRelativeLocation(FVector(0.686449f, -9.618860f, 2.343055f));
	compMeshLollipop->SetRelativeRotation(FRotator(0, 0, -20));
	compMeshLollipop->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//[�� ����ƽ�޽� ������Ʈ �߰�]
	compMeshShovel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShovelMeshComp"));
	compMeshShovel->SetupAttachment(GetMesh(), TEXT("Shovel"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempShoveMesh(TEXT("StaticMesh'/Game/Geometry/SM_shovel.SM_shovel'"));
	if (tempShoveMesh.Succeeded())
	{
		compMeshShovel->SetStaticMesh(tempShoveMesh.Object);
	}
	compMeshShovel->SetRelativeLocation(FVector(-5, 5, 25));
	compMeshShovel->SetRelativeRotation(FRotator(10, 1, -76));
	compMeshShovel->SetRelativeScale3D(FVector(0.8f));
	compMeshShovel->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//[�״Ͻ����� ����ƽ�޽� ������Ʈ �߰�]
	compMeshTennis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TennisMeshComp"));
	compMeshTennis->SetupAttachment(GetMesh(), TEXT("Tennis"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempTennisMesh(TEXT("StaticMesh'/Game/Geometry/Tenis.Tenis'"));
	if (tempTennisMesh.Succeeded())
	{
		compMeshTennis->SetStaticMesh(tempTennisMesh.Object);
	}
	compMeshTennis->SetRelativeLocation(FVector(-5, 11, 25));
	compMeshTennis->SetRelativeRotation(FRotator(10, 1, -76));
	compMeshTennis->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//[�ҷ� Ŭ���� �����ͼ� ���. �ҷ����丮 �ȿ� �ҷ� ����Ʈ�� �߰�] <�ҷ��� ������ ��� ���� ���� ���ϴ� ���� �����Ϸ��� ������ ��� ���� ��>
	ConstructorHelpers::FClassFinder<ARIM_Bullet> tempBul(TEXT("Blueprint'/Game/BluePrint/BP_RIM_Bullet.BP_RIM_Bullet_C'"));
	if (tempBul.Succeeded())
	{
		bulletFactory = tempBul.Class;
	}

	//[���� ����Ҷ� ������ �� Ŭ���� �����ͼ� ���]
	ConstructorHelpers::FClassFinder<AGunWeapon> tempGun(TEXT("Class'/Script/DogFight.GunWeapon'"));
	if (tempGun.Succeeded())
	{
		weaponGun = tempGun.Class;
	}

	//[���� ����Ҷ� ������ �Ѹ��� Ŭ���� �����ͼ� ���]
	ConstructorHelpers::FClassFinder<ALollipopWeapon> tempLollipop(TEXT("Class'/Script/DogFight.LollipopWeapon'"));
	if (tempLollipop.Succeeded())
	{
		weaponLollipop = tempLollipop.Class;
	}

	//[���� ����� �� ������ �� Ŭ���� �����ͼ� ���]
	ConstructorHelpers::FClassFinder<AShovelWeapon> tempShovel(TEXT("Class'/Script/DogFight.ShovelWeapon'"));
	if (tempShovel.Succeeded())
	{
		weaponShovel = tempShovel.Class;
	}

	//[���� ����Ҷ� ������ �״Ͻ����� �����ͼ� ���]
	ConstructorHelpers::FClassFinder<ATennisWeapon> tempTennis(TEXT("Class'/Script/DogFight.ShovelWeapon'"));
	if (tempTennis.Succeeded())
	{
		weaponTennis = tempTennis.Class;
	}

	//[�÷��̾� ABP_�ִϸ��̼� Ŭ���� �����ͼ� ���]
	ConstructorHelpers::FClassFinder<URIM_PlayerAnim> tempAnim(TEXT("AnimBlueprint'/Game/BluePrint/ABP_Player.ABP_Player_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class); //�÷��̾�->�޽�->�ִϸ��̼�->�ִ�Ŭ���� �� �߰�. Anim Class �� ��Ӵٿ����� ABP_Player �߰�
	}

	PhysicComp = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnim"));

	DeadBlock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DeadBlock"));
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempCube(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempCube.Succeeded())
	{
		DeadBlock->SetStaticMesh(tempCube.Object);
	}
	DeadBlock->SetupAttachment(GetMesh());
	DeadBlock->SetRelativeLocation(FVector(0, -30, 170));
	DeadBlock->SetRelativeScale3D(FVector(0.75f, 1.5f, 2.0f));
	DeadBlock->SetCollisionResponseToAllChannels(ECR_Ignore);
	DeadBlock->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Block);
	DeadBlock->SetVisibility(false);
}

// Called when the game starts or when spawned
void ARIM_Player::BeginPlay()
{
	Super::BeginPlay();

	//[�ʱ� �ӵ��� �ȱ�� ����]
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	//[���� �����Ҷ� �Ⱥ��̰� ����]
	compMeshGun->SetVisibility(false);

	//[�Ѹ����� �����Ҷ� �� ���̰� ����]
	compMeshLollipop->SetVisibility(false);
	
	//[���� �����Ҷ� �� ���̰� ����]
	compMeshShovel->SetVisibility(false);

	//[�״Ͻ����� �����Ҷ� �� ���̰� ����]
	compMeshTennis->SetVisibility(false);

	//�ִϸ��̼� ���� <�޽��� ���õ� �ִϸ��̼��� ������ ��� ���� ��> �ڡڡھ��� ���� �ȵ�
	animPlayer = Cast<URIM_PlayerAnim>(GetMesh()->GetAnimInstance());

	//[���忡 ������ ��� ���ʹ� ��̿� ���] �ڡڡڿ� �ʿ�?
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASH_Enemy::StaticClass(), enemyActor);
	for (int32 i = 0; i < enemyActor.Num(); i++)
	{
		ASH_Enemy* currentenemy = Cast <ASH_Enemy>(enemyActor[i]);
		enemyarray.Add(currentenemy);
	}

	setPhysicsData();
}

void ARIM_Player::setPhysicsData()
{

	FPhysicalAnimationData PhysicalAnimDate;
	PhysicalAnimDate.bIsLocalSimulation = true;
	PhysicComp->StrengthMultiplyer = 1.0f;
	PhysicalAnimDate.OrientationStrength = 5000.0f;
	PhysicalAnimDate.AngularVelocityStrength = 100.0f;
	PhysicalAnimDate.PositionStrength = 5000.0f;
	PhysicalAnimDate.VelocityStrength = 100.0f;
	PhysicalAnimDate.MaxAngularForce = 100.0f;
	PhysicalAnimDate.MaxLinearForce = 100.0f;

	PhysicComp->SetSkeletalMeshComponent(GetMesh());
	PhysicComp->ApplyPhysicalAnimationSettingsBelow(TEXT("spine_01"), PhysicalAnimDate, true);
	GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("spine_01"), true);
}

// Called every frame
void ARIM_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//[Move(�÷��̾� �̵�) �Լ� ȣ��]
	Move();

	//[�÷��̾� �˴ٿ�(����) �� �Ͼ�� �ڵ�]
	if (playerState == EPlayerState::KnockDown) //�÷��̾ ����������
	{
		currentTime += DeltaTime; //����ð� + ��ŸŸ�� = ���� �����ð�

		if (currentTime > 5) //�÷��̾ ������ �ð�(5��)�� ������
		{
			//�÷��̾� �ִϸ��̼� ��Ÿ�� �� '��' �ִϸ��̼� ���� ���
			rand = FMath::RandRange(0, 1);
			animPlayer->PlayPlayerTwoAnim(TEXT("WakeUp"), rand);

			UE_LOG(LogTemp, Error, TEXT("Player Wake up!")); //Ȯ�ο� �ؽ�Ʈ ���

			currentTime = 0; //���� �ð��� 0�ʷ� �ʱ�ȭ �Ѵ�
			HP = 5; //�÷��̾��� HP�� 5�ʷ� �ʱ�ȭ �Ѵ�

			//isplayerDown = false;//�÷��̾ �������ִ��� Ȯ���ϴ� ���� �ʱ�ȭ. �÷��̾� �Ͼ�� 
			playerState = EPlayerState::Idle;
		}
	}

	//[���� ���� ����]
	if (playerState == EPlayerState::Die) //�÷��̾ ���� ������ ��
	{
		currentTime += DeltaTime;

		if (currentTime > 2) //2�� �Ŀ�
		{
			UGameplayStatics::OpenLevel(GetWorld(), "Result"); //��� ����(ȭ��)�� ����
		}
	}

	if (GetVelocity().Length() > 650) //���ν�Ƽ�� 650�̻��̸� ������ 600���� �����ش�.
	{
		FVector v = GetVelocity().GetSafeNormal();
		GetCharacterMovement()->Velocity = v * 600;
	}
	if (playerState == EPlayerState::KnockDown)
	{
		GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("spine_01"), false);
	}
	else
	{
		GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("spine_01"), true);
	}
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

	//[����/��� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��] = �Ѿ� �߻� + ��ġ + ���� ����
	PlayerInputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, &ARIM_Player::InputPunchGrab);

	//[���ű/������ �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]
	PlayerInputComponent->BindAction(TEXT("KickToss"), IE_Pressed, this, &ARIM_Player::InputKickToss);

	//[��ġ�� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]
	PlayerInputComponent->BindAction(TEXT("Headbutt"), IE_Pressed, this, &ARIM_Player::InputHeadbutt);

	//[���� ������ �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]
	PlayerInputComponent->BindAction(TEXT("DropWeapon"), IE_Pressed, this, &ARIM_Player::InputDropWeapon);
}

//[Move(�÷��̾� �̵�) �Լ� ����]
void ARIM_Player::Move()
{
	//if (isplayerDown == true || isplayerDie == true)  return; //�÷��̾ ������� �� �� �̵� �����ϰ� �Ѵ�. �÷��̾ ���� �ְų�, �׾������� Move �Լ��� �������� �ʴ´�.
	if (playerState == EPlayerState::KnockDown || playerState == EPlayerState::Die) return;

	//[�÷��̾� �̵� ���� �ڵ�]
	direction = FTransform(GetControlRotation()).TransformVector(direction); //�̵� ������ ��Ʈ�� ���� �������� ��ȯ
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
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
	if (playerState == EPlayerState::KnockDown || playerState == EPlayerState::Die) return;
	{
		Jump();
	}
}


//[���� �̺�Ʈ ó�� �Լ� ����] = �Ѿ� �߻� + ��ġ + ���� ����
void ARIM_Player::InputPunchGrab()
{
	//if (isplayerDown == true || isplayerDie == true)  return; //�÷��̾ ����������� ������ �� �ְ� �Ѵ�. �÷��̾ �����ְų�, �׾������� InputPunchGrab �Լ��� �������� �ʴ´�.
	if (playerState == EPlayerState::KnockDown || playerState == EPlayerState::Die) return;

	if (compMeshGun->IsVisible()) //�÷��̾ ���� ��� ������ -> �ѿ��� �Ѿ��� �߻��Ѵ�
	{
		animPlayer->PlayPlayerAnim(TEXT("Fire"), 0); //�÷��̾� �ִϸ��̼� ��Ÿ�� �� '���̾�' �ִϸ��̼� ���
		GetWorld()->SpawnActor<ARIM_Bullet>(bulletFactory, GetActorLocation() + GetActorForwardVector() * 200, GetActorRotation()); //�Ѿ� ����
	}
	else if (compMeshLollipop->IsVisible()) //�÷��̾ ���� ��� ���� �ʰ�, �Ѹ����� ��� ������ -> �Ѹ������� �����Ѵ�
 	{
		animPlayer->PlayPlayerTwoAnim(TEXT("Lollipop"), 0); //�÷��̾� �ִϸ��̼� ��Ÿ�� �� '�Ѹ���' �ִϸ��̼� ���
		TargetDotAttack();
	}
	else if (compMeshShovel->IsVisible()) //�÷��̾ ��, �Ѹ����� ��� ���� �ʰ�, ���� ��� ������ -> ������ �����Ѵ�
	{
		animPlayer->PlayPlayerAnim(TEXT("Swing"), 0); //�÷��̾� �ִϸ��̼� ��Ÿ�� �� '��' �ִϸ��̼� ���
		TargetDotAttack();
	}
	else if (compMeshTennis->IsVisible()) //�÷��̾ ��, �Ѹ���, ���� ��� ���� �ʰ�, �״Ͻ� ������ ��� ������ -> �״Ͻ� �������� �����Ѵ�
	{ 
		animPlayer->PlayPlayerTwoAnim(TEXT("Lollipop"), 0); //�÷��̾� �ִϸ��̼� ��Ÿ�� �� '�״Ͻ�' �ִϸ��̼� ���
		TargetDotAttack();
	}
	else //�÷��̾ ��, �Ѹ���, �� ��� ���� �ʰ� InputPunchGrab ��ư�� ������ -> ��ġ�Ѵ� 
	{
		animPlayer->PlayPlayerAnim(TEXT("Swing"), 0); //�÷��̾� �ִϸ��̼� ��Ÿ�� �� '��ġ' �ִϸ��̼� ���
		TargetDotAttack();
	}
}


//############### ű ###############
// [���ű/������ �̺�Ʈ ó�� �Լ� ����]
void ARIM_Player::InputKickToss()
{
	//if (isplayerDown == true || isplayerDie == true)  return; //�÷��̾ ����������� ������ �� �ְ� �Ѵ�. �÷��̾ �����ְų�, �׾������� InputKickToss �Լ��� �������� �ʴ´�.
	if (playerState == EPlayerState::KnockDown || playerState == EPlayerState::Die) return;

	animPlayer->PlayPlayerTwoAnim(TEXT("Kick"), 0); //�÷��̾� �ִϸ��̼� ��Ÿ�� �� 'ű' �ִϸ��̼� ���
	TargetDotAttack();
}


//############### ��ġ�� ###############
// [��ġ�� �̺�Ʈ ó�� �Լ� ����]
void ARIM_Player::InputHeadbutt() 
{
	//if (isplayerDown == true || isplayerDie == true)  return; //�÷��̾ ����������� ������ �� �ְ� �Ѵ�. �÷��̾ �����ְų�, �׾������� InputKickToss �Լ��� �������� �ʴ´�.
	if (playerState == EPlayerState::KnockDown || playerState == EPlayerState::Die) return;

	animPlayer->PlayPlayerAnim(TEXT("Headbutt"), 0); //�÷��̾� �ִϸ��̼� ��Ÿ�� �� '���' �ִϸ��̼� ���
	TargetDotAttack();
}


//[�÷��̾ ���� ���̰� �ϴ� �Լ� ����]
void ARIM_Player::VisibleGun()
{
	if (compMeshLollipop->IsVisible() || compMeshShovel->IsVisible() || compMeshTennis->IsVisible()) return; //�Ѹ��� or �� or �״Ͻ������� ��� ���� �� �� �� ��� �Ѵ�.
	compMeshGun->SetVisibility(true); //�÷��̾ �� ���� ���̰� �Ѵ�
}

//[�÷��̾ �Ѹ����� ���̰� �ϴ� �Լ� ����]
void ARIM_Player::VisibleLollipop()
{
	if (compMeshGun->IsVisible() || compMeshShovel->IsVisible() || compMeshTennis->IsVisible()) return; //�� or �� or �״Ͻ������� ��� ���� �� �Ѹ��� �� ��� �Ѵ�.
	compMeshLollipop->SetVisibility(true);
}

//[�÷��̾ ���� ���̰� �ϴ� �Լ� ����]
void ARIM_Player::VisibleShovel()
{
	if (compMeshGun->IsVisible() || compMeshLollipop->IsVisible() || compMeshTennis->IsVisible()) return; //�� or �Ѹ��� or �״Ͻ������� ��� ���� �� �� �� ��� �Ѵ�.
	compMeshShovel->SetVisibility(true);
}

//[�÷��̾ �״Ͻ��� ���̰� �ϴ� �Լ� ����]
void ARIM_Player::VisibleTennis()
{
	if (compMeshGun->IsVisible() || compMeshLollipop->IsVisible() || compMeshShovel->IsVisible()) return //�� or �Ѹ��� or ���� ��� ���� �� �״Ͻ������� �� ��� �Ѵ�.
	compMeshTennis->SetVisibility(true);
}


//[���� ������ �̺�Ʈ ó�� �Լ� ����]
void ARIM_Player::InputDropWeapon()
{
	AItemSpawn* ItemSpawn = Cast<AItemSpawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AItemSpawn::StaticClass())); //�ڡڡ�???
	if (compMeshGun->IsVisible()) //�÷��̾ ���� ��� ������
	{
		compMeshGun->SetVisibility(false); //�÷��̾��� �� ������Ʈ�� �Ⱥ��̰��Ѵ�
		ItemSpawn->CreateWeapon(int32(EWeaponType::Gun), GetActorLocation() + FVector(0, 50, 50), GetActorRotation()); //GunWeapon�� �÷��̾� ��ġ�� �ٴڿ� �����ȴ�. //�ڡڡھ ����???
		UE_LOG(LogTemp, Error, TEXT("Player Gun Drop!")); //Ȯ�ο� �ؽ�Ʈ ���
	}
	else if (compMeshLollipop->IsVisible()) //�÷��̾ �Ѹ����� ��� ������
	{
		compMeshLollipop->SetVisibility(false);
		ItemSpawn->CreateWeapon(int32(EWeaponType::Lollipop), GetActorLocation() + FVector(0, 50, 50), GetActorRotation()); //�ڡڡھ ����???
		UE_LOG(LogTemp, Error, TEXT("Player Lollipop Drop!"));
	}
	else if (compMeshShovel->IsVisible()) //�÷��̾ ���� ��� ������
	{
		compMeshShovel->SetVisibility(false);
		ItemSpawn->CreateWeapon(int32(EWeaponType::Shovel), GetActorLocation() + FVector(0, 50, 50), GetActorRotation()); //�ڡڡھ ����???
		UE_LOG(LogTemp, Error, TEXT("Player Shovel Drop!"));
	}
	else if (compMeshTennis->IsVisible()) //�÷��̾ �״Ͻ������� ��� ������
	{
		compMeshTennis->SetVisibility(false);
		ItemSpawn->CreateWeapon(int32(EWeaponType::Tennis), GetActorLocation() + FVector(0, 50, 50), GetActorRotation()); //�ڡڡھ ����???
		UE_LOG(LogTemp, Error, TEXT("Player Tennis Drop!"));
	}
}


//[�÷��̾� HP ���� �Լ�] �÷��̾ ���� ���ϸ� ���ʹ̿��� ȣ���ϴ� �Լ�. �÷��̾��� �������ϱ� �÷��̾�� ����
void ARIM_Player::OnDamageProcess()
{
	HP--; //�÷��̾� HP 1�� ����
	UE_LOG(LogTemp, Error, TEXT("%d"), HP); //Ȯ�ο� �ؽ�Ʈ ���

	if (HP > 0)
	{
		//�÷��̾� �ִϸ��̼� ��Ÿ�� �� '������' �ִϸ��̼� ���� ���
		rand = FMath::RandRange(0, 2);
		animPlayer->PlayPlayerTwoAnim(TEXT("Damaged"), rand);
		//���� ����Ʈ���� �Լ� ȣ��
		InputDropWeapon();
		UE_LOG(LogTemp, Error, TEXT("Player Damaged!")); //Ȯ�ο� �ؽ�Ʈ ���
	}
	else if (HP <= 0) //���� ���ʹ����� 5�� ���� ������(HP�� 5�̰� ���� ���������� HP�� 1�� �پ��ϱ� 5�� ���� �޴� ���� HP�� 0�� �Ǵ� �Ͱ� �����ϴ�)
	{
		//�÷��̾� �ִϸ��̼� ��Ÿ�� �� '�˴ٿ�' �ִϸ��̼� ���� ���
		rand = FMath::RandRange(0, 1);
		animPlayer->PlayPlayerTwoAnim(TEXT("Knockdown"), rand); //5�� �� �Ͼ�� �ڵ�� Tick���� �����Ѵ�. �ð��� �귯�� �ϴϱ�
		//���� ����Ʈ���� �Լ� ȣ��
		InputDropWeapon();

		//�÷��̾� ����. ���� ��
		//isplayerDown = true;
		playerState = EPlayerState::KnockDown;

		UE_LOG(LogTemp, Error, TEXT("Player Knockdown!")); //Ȯ�ο� �ؽ�Ʈ ���
	}
}


//[�÷��̾� ����]
void ARIM_Player::Die()
{
	//�÷��̾� �ִϸ��̼� ��Ÿ�� �� '����' �ִϸ��̼� ���� ���
	rand = FMath::RandRange(0, 1);
	animPlayer->PlayPlayerTwoAnim(TEXT("Die"), rand);

	ADogFightGameModeBase* gamemodefail = GetWorld()->GetAuthGameMode<ADogFightGameModeBase>(); //���Ӹ�带 �����´�.
	gamemodefail->addtoViewfail(); //���Ӹ���� Fail ���� ���� �Լ��� ȣ���Ѵ�.

	//isplayerDie = true; //�÷��̾� ���� 
	playerState = EPlayerState::Die;
}


//[��Ʈ. �÷��̾��� �þ߾ȿ��� ���̴� ���ʹ��� �Ÿ��� �缭 ������ �°��ϴ� �Լ�] �ڡڡڡڡ�
void ARIM_Player::TargetDotAttack()
{
	for (int32 i = 0; i < enemyarray.Num(); i++)
	{
		FVector dir = enemyarray[i]->GetActorLocation() - GetActorLocation(); // ����. �÷��̾�� ���ʹ̸� ���ϴ� ���� ���ϱ�
		float dotValue = FVector::DotProduct(GetActorForwardVector(), dir.GetSafeNormal()); //����. �÷��̾��� �չ���� ������ ���� ����(��ֶ����� �ʿ�)�� ����
		float angle = UKismetMathLibrary::DegAcos(dotValue); //�������� ���ϱ�. ������ ���� ���� Acos �ϱ�
		if (angle < 30 && dir.Length() < 300) //�÷��̾� �þ� 60�� ���̿� ���ʹ� �Ÿ��� 300�̸�
		{	
			if (enemyarray[i]->fsm->mState != EEnemyState::Down && enemyarray[i]->fsm->mState != EEnemyState::Die && enemyarray[i]->fsm->mState != EEnemyState::Damage) //���ʹ��� ���¸� Ȯ���ϰ�(�˴ٿ�+����+������ ���°� �ƴϸ�)
			{
				enemyarray[i]->fsm->OnDamageProcess(); //���ʹ̿��� �������� �ش�.
			}
		}
	}
}