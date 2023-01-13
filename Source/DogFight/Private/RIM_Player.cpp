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

// Sets default values
ARIM_Player::ARIM_Player() //������
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[�÷��̾� ���̷�Ż�޽� �߰�. �� �߰�]
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor2.Charactor2'")); //�� ���� ���� ������ ���ε�
	if (tempMesh.Succeeded()) //���� ���ε� ������
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object); //ĳ���� �޽��� ���̷�Ż�޽ÿ� ���� ���ε�. �� ���� 
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	//[�÷��̾� ��ü �浹 ������Ʈ Collision ����]
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player")); //������ Player �� ���� 
	
	

	//[�������� ������Ʈ �߰�. USpringArmComponent]
	compSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	compSpringArm->SetupAttachment(RootComponent); //��Ʈ������Ʈ�� �ڽ����� ����
	compSpringArm->SetRelativeLocation(FVector(0, 0, 0)); //������ �ʿ�� ����
	compSpringArm->SetRelativeRotation(FRotator(0, 0, 0)); //������ �ʿ�� ����
	compSpringArm->TargetArmLength = 900; //������ �ʿ�� ����
	compSpringArm->SocketOffset = FVector(0, 0, 450); //������ �ʿ�� ����
	compSpringArm->bUsePawnControlRotation = true; //�Է¿� ���� ȸ�� ����

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
	compMeshGun->SetupAttachment(GetMesh(), TEXT("Gun")); //�θ� ������Ʈ�� Mesh ������Ʈ�� ����. ���� ĳ���� �� ���Ͽ� ����...?
		
	//[�� ����ƽ�޽� �߰�. �� �߰�] 
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempGunMesh(TEXT("StaticMesh'/Game/Geometry/Gun.Gun'")); 
	if (tempGunMesh.Succeeded())
	{
		compMeshGun->SetStaticMesh(tempGunMesh.Object);
		//compMeshGun->SetRelativeLocation(FVector(-8.280492f, 21.242059f, 5.475584f)); //���� ��ġ. �ӽ÷� ����. ���� ���� �ʿ�
		//compMeshGun->SetRelativeRotation(FRotator(60.000000f, -339.000000f, -270.000000f)); //���� ȸ��. �ӽ÷� ����. ���� ���� �ʿ�
	}
	
	

	//[�Ѹ��� ����ƽ�޽� ������Ʈ �߰�]
	compMeshLollipop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LollipopMeshComp"));
	compMeshLollipop->SetupAttachment(GetMesh(), TEXT("Lollipop")); //�θ� ������Ʈ�� Mesh ������Ʈ�� ����. �Ѹ����� ĳ���� �� ���Ͽ� ����...?

	//[�Ѹ��� ����ƽ�޽� �߰�. �� �߰�]
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempLollipopMesh(TEXT("StaticMesh'/Game/Geometry/Lollipop.Lollipop'"));
	if (tempLollipopMesh.Succeeded())
	{
		compMeshLollipop->SetStaticMesh(tempLollipopMesh.Object);
		compMeshLollipop->SetRelativeLocation(FVector(0.686449f, -9.618860f, 2.343055f));
		compMeshLollipop->SetRelativeRotation(FRotator(0, 0, -20));
	}

	//[�Ѹ��� �浹ü ������Ʈ �߰�]
	compCollisionLollipop = CreateDefaultSubobject<USphereComponent>(TEXT("LollipopCollisionComp"));
	compCollisionLollipop->SetupAttachment(compMeshLollipop); //�ڡڡڿ����� ���� �ȵ�
	compCollisionLollipop->SetSphereRadius(35); //������ �ʿ�� ����
	compCollisionLollipop->SetRelativeLocation(FVector(0.5f, 0, 40)); //������ �ʿ�� ���� 



	//[�÷��̾� ��ġ �浹 ������Ʈ �߰�]
	compCollisionPunchR = CreateDefaultSubobject<UBoxComponent>(TEXT("PunchCollisionCompR"));
	compCollisionPunchL = CreateDefaultSubobject<UBoxComponent>(TEXT("PunchCollisionCompL"));
	compCollisionPunchR->SetRelativeLocation(FVector(0, 6, -9));
	compCollisionPunchR->SetRelativeScale3D(FVector(0.6f, 0.5f, 0.5f));
	compCollisionPunchL->SetRelativeScale3D(FVector(0.6f, 0.5f, 0.5f));
	compCollisionPunchR->SetupAttachment(GetMesh(), TEXT("Prop_RSocket")); //�޽��� �ڽ����� ����. ������. �浹 ������Ʈ�� ĳ���� ���Ͽ� ����
	compCollisionPunchL->SetupAttachment(GetMesh(), TEXT("Prop_LSocket")); //�޽��� �ڽ����� ����. �޼�. �浹 ������Ʈ�� ĳ���� ���Ͽ� ����

 	//[�÷��̾� ű �浹 ������Ʈ �߰�]
 	compCollisionKick = CreateDefaultSubobject<UBoxComponent>(TEXT("KickCollisionComp"));
	compCollisionKick->SetRelativeLocation(FVector(26, 10, 6));
	compCollisionKick->SetRelativeScale3D(FVector(0.8f, 0.8f, 1));
 	compCollisionKick->SetupAttachment(GetMesh(), TEXT("ball_lSocket")); //�޽��� �ڽ����� ����. �޹�. �浹 ������Ʈ�� ĳ���� ���Ͽ� ����
 
	//[�÷��̾� ��� �浹 ������Ʈ �߰�]
 	compCollisionHeadbutt = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadbuttCollisionComp"));
	compCollisionHeadbutt->SetRelativeLocation(FVector(0, 48, 193));
 	compCollisionHeadbutt->SetRelativeScale3D(FVector(1.5f, 1.7f, 1.2f));
 	compCollisionHeadbutt->SetupAttachment(GetMesh(), TEXT("headSocket")); //�޽��� �ڽ����� ����. �Ӹ�. �浹 ������Ʈ�� ĳ���� ���Ͽ� ����



	//[�ҷ����丮 �ȿ� �ҷ� ����Ʈ�� �߰�] �ڡڡڿ� �� �۾��� �ʿ����� �ñ�
	ConstructorHelpers::FClassFinder<ARIM_Bullet> tempBul(TEXT("Blueprint'/Game/BluePrint/BP_RIM_Bullet.BP_RIM_Bullet_C'")); //�ڡڡڿ���. ��� �������� �ּ�ó���ϰ� �������Ʈ���� ���� �� ��
	if(tempBul.Succeeded())
	{
		bulletFactory = tempBul.Class; //�÷��̾ �߰�. Bullet Factory �� ��Ӵٿ����� BP_RIM_Bullet �߰�
	}

	//[�� ���� ������ �߰�]
	ConstructorHelpers::FClassFinder<AGunWeapon> tempGun(TEXT("Class'/Script/DogFight.GunWeapon'"));
	if (tempGun.Succeeded())
	{
		weaponGun = tempGun.Class; //�÷��̾ �߰�. WeaponGun �� ��Ӵٿ����� GunWeapon �߰�
	}

	//[�Ѹ��� ���� ������ �߰�]
	ConstructorHelpers::FClassFinder<ALollipopWeapon> tempLollipop(TEXT("Class'/Script/DogFight.LollipopWeapon'"));
	if (tempLollipop.Succeeded())
	{
		weaponLollipop = tempLollipop.Class; //�÷��̾ �߰�. WeaponLollipop �� ��Ӵٿ����� LollipopWeapon �߰�
	}

	//[�ִϸ��̼� ����Ʈ�� ����]
	ConstructorHelpers::FClassFinder<URIM_PlayerAnim> tempAnim(TEXT("AnimBlueprint'/Game/BluePrint/ABP_Player.ABP_Player_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class); //�÷��̾�->�޽�->�ִϸ��̼�->�ִ�Ŭ���� �� �߰�. Anim Class �� ��Ӵٿ����� ABP_Player �߰�
	}




}

// Called when the game starts or when spawned
void ARIM_Player::BeginPlay()
{
	Super::BeginPlay();

	//[�ʱ� �ӵ��� �ȱ�� ����]
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	//[���� ����Ʈ�� �Ⱥ��̰� ����]
	compMeshGun->SetVisibility(false); //������ �� �÷��̾ ���� ��� ���� �ʴ´�. ���� �� ���δ�. 

	//[�Ѹ����� ����Ʈ�� �� ���̰� ����]
	compMeshLollipop->SetVisibility(false); //������ �� �÷��̾ �Ѹ����� ��� ���� �ʴ´�. �Ѹ����� �� ���δ�.


	//[[1]�÷��̾� �ָ� �ݸ����� ���ʹ̰� �浹 �� ���� �� �Լ� ȣ��] �ڡڡڿϺ��ϰ� �������� �� ��. �� BeginPlay���� �ϴ���???
	//��ġ �ݸ��� ������Ʈ ������ ��� �Լ� OnComponentBeginOverlap�� OnComponentEndOverlap ��������Ʈ ȣ�� + ��ġ �ݸ��� ������Ʈ�� �ؿ� ������ �Լ� ����
	compCollisionPunchR->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionPunchRBeginOverlap);
	compCollisionPunchL->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionPunchLBeginOverlap);
	compCollisionPunchR->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionPunchREndOverlap);
	compCollisionPunchL->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionPunchLEndOverlap);

	//[1]�÷��̾� ű �ݸ����� ���ʹ̰� �浹 �� ���� �� �Լ� ȣ��
	compCollisionKick->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionKickBeginOverlap);
	compCollisionKick->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionKickEndOverlap);

	//[1]�÷��̾� ��ġ�� �ݸ����� ���ʹ̰� �浹 �� ���� �� �Լ� ȣ��
	compCollisionHeadbutt->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionHeadbuttBeginOverlap);
	compCollisionHeadbutt->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionHeadbuttEndOverlap);

	//[1]�÷��̾ �� �Ѹ��� �ݸ����� ���ʹ̰� �浹 �� ���� �� �Լ� ȣ��
	compCollisionLollipop->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisonLollipopBeginOverlap);
	compCollisionLollipop->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisonLollipopEndOverlap);

	//�ִϸ��̼� ���� �ڡڡ�???
	animPlayer = Cast<URIM_PlayerAnim>(GetMesh()->GetAnimInstance());

}

// Called every frame
void ARIM_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//[Move(�÷��̾� �̵�) �Լ� ȣ��]
	Move();

	//[�÷��̾� ���� �� �Ͼ�� �ڵ�]
	if (isplayerDie == false && isplayerDown == true) //�÷��̾ ����ְ� + �÷��̾ ����������
	{
		currentTime += GetWorld()->DeltaTimeSeconds; //����ð� + ��ŸŸ�� = ���� �����ð�

		if (currentTime > 5) //�÷��̾ ������ �ð�(5��)�� ������
		{
			//�÷��̾� �ִϸ��̼� ��Ÿ�� �� '��' �ִϸ��̼� ���� ���
			rand = FMath::RandRange(0, 1);
			animPlayer->PlayPlayerTwoAnim(TEXT("WakeUp"), rand);

			UE_LOG(LogTemp, Error, TEXT("Player Wake up!")); // Ȯ�ο� �ؽ�Ʈ ���
						
			currentTime = 0; //���� �ð��� 0�ʷ� �ʱ�ȭ �Ѵ�
			HP = 5; //�÷��̾��� HP�� 5�ʷ� �ʱ�ȭ �Ѵ�

			isplayerDown = false; //�÷��̾ �Ͼ���Ѵ�. Ű ������ �����̰� �ϱ� ���� �߰�
		}
	}

	//[���� ���� ����]
	if (isplayerDie == true) //�÷��̾ ���� ������ ��
	{
		currentTime += DeltaTime;

		if (currentTime > 2)
		{
			UGameplayStatics::OpenLevel(GetWorld(), "Result"); //��� ����(ȭ��)�� ����.
		}
	}
}

//[Move(�÷��̾� �̵�) �Լ� ����]
void ARIM_Player::Move()
{
	if (isplayerDown == false) //�÷��̾ ������ ���� �̵� �����ϰ� �Ѵ�
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
		AddMovementInput(direction);
		direction = FVector::ZeroVector;	
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

		//[����/��� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��] = �Ѿ� �߻� + ��ġ
		PlayerInputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, &ARIM_Player::InputPunchGrab);
	
		//[���ű/������ �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]
		PlayerInputComponent->BindAction(TEXT("KickToss"), IE_Pressed, this, &ARIM_Player::InputKickToss);
	
		//[��ġ�� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]
		PlayerInputComponent->BindAction(TEXT("Headbutt"), IE_Pressed, this, &ARIM_Player::InputHeadbutt);

		//[���� ������ �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]
		PlayerInputComponent->BindAction(TEXT("DropWeapon"), IE_Pressed, this, &ARIM_Player::InputDropWeapon);
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

// 	ACharacterSpawn* spawn = Cast<ACharacterSpawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterSpawn::StaticClass()));
// 	ASH_Enemy* enemy = spawn->spawnedEnemy[spawn->spawnedEnemy.Num() - 1];
// 	enemy->fsm->stateChangeMontage(EEnemyState::Die, TEXT("Die"));
// 	spawn->spawnedEnemy.Remove(enemy);
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


//############### �Ѿ� �߻�, �Ѹ��� + ��ġ ###############
//[����/��� �̺�Ʈ ó�� �Լ� ����] = �Ѿ� �߻� + �Ѹ��� + ��ġ
void ARIM_Player::InputPunchGrab()
{	
	if (isplayerDown == false) //�÷��̾ ������ ��
	{
		if (compMeshGun->IsVisible() == true) //���� �÷��̾ ���� ��� ������(�÷��̾ �� ���� ���̸�) -> �ѿ��� �Ѿ��� �߻��Ѵ�
		{
			UE_LOG(LogTemp, Error, TEXT("Player Fire!!!")); //Ȯ�ο� �ؽ�Ʈ ���

			//�÷��̾� �ִϸ��̼� ��Ÿ�� �� '���̾�' �ִϸ��̼� ���
			animPlayer->PlayPlayerAnim(TEXT("Fire"), 0);

			//�Ѿ� �߻�
			//FTransform trFire = compMeshGun->GetSocketTransform(TEXT("FirPos")); //�� ���̷��濡�� ���� �߰� ---> ó���� �� ���̷���޽��� �� ��� �ߴ� �ڵ�
			//GetWorld()->SpawnActor<ARIM_Bullet>(bulletFactory, trFire); ////���� �ѱ����� �Ѿ��� �߻��Ѵ�. ���忡�� ARIM_Bullet(�Ѿ�) �����´� ---> ó���� �� ���̷���޽��� �� ��� �ߴ� �ڵ�

			GetWorld()->SpawnActor<ARIM_Bullet>(bulletFactory, GetActorLocation() + GetActorForwardVector() * 200, GetActorRotation());
		}
		else if (compMeshLollipop->IsVisible() == true) //�÷��̾ �Ѹ����� ��� ���� �ʰ�, �Ѹ����� ��� ������(�÷��̾ �� �Ѹ����� ���̸�) -> �Ѹ������� �����Ѵ�
		{
			UE_LOG(LogTemp, Error, TEXT("Player Lollipop!")); //Ȯ�ο� �ؽ�Ʈ ���

			//�÷��̾� �ִϸ��̼� ��Ÿ�� �� '�Ѹ���' �ִϸ��̼� ���
			animPlayer->PlayPlayerTwoAnim(TEXT("Lollipop"), 0);

			if (isInputPunchGrab == true) //�÷��̾ �Ѹ����� �� ���·� isInputPunchGrab Ű�� ������
			{
				UE_LOG(LogTemp, Error, TEXT("Player Lollipop Attack!")); //Ȯ�ο� �ؽ�Ʈ ���

				Enemy = Cast<ASH_Enemy>(currEnemy); //���ʹ� ����ȯ! ���� �� ������ �ƴ��� �� üũ�ؾ� �Ѵ�
				if (Enemy != nullptr) //���ʹ̰� �ƴϴ� = ���ʹ��̴�. �÷��̾� �Ѹ��˿� ���� ���� ���ʹ̰� �ƴ� ���� �ֱ� ������ ���ʹ����� Ȯ�� �ʿ�
				{
					//���ʹ̰� ���, �̵�, ����, ���� ��� ���� �� ��
					if (Enemy->fsm->mState == EEnemyState::Idle || Enemy->fsm->mState == EEnemyState::Move || Enemy->fsm->mState == EEnemyState::Attack || Enemy->fsm->mState == EEnemyState::Pickup)
					{
						Enemy->fsm->OnDamageProcess(); //���ʹ� ������ �������� ����. ���ʹ�(���� ��� ����)���� fsm(���� ������Ʈ ��� ����)���� ���������μ��� ȣ��
					}
				}
			}
		}
		else //�÷��̾ ���� ��� ���� �ʰ�, �Ѹ����� ��� ���� �ʰ� InputPunchGrab ��ư�� ������ -> ��ġ�Ѵ� 
		{
			UE_LOG(LogTemp, Error, TEXT("Player Punch!")); //Ȯ�ο� �ؽ�Ʈ ���

			//�÷��̾� �ִϸ��̼� ��Ÿ�� �� '��ġ' �ִϸ��̼� ���
			animPlayer->PlayPlayerAnim(TEXT("Punch"), 0);

			if (isInputPunchGrab == true) //isInputPunchGrab Ű�� ������
			{
				UE_LOG(LogTemp, Error, TEXT("Player Punch Attack!")); //Ȯ�ο� �ؽ�Ʈ ���

				Enemy = Cast<ASH_Enemy>(currEnemy); //���ʹ� ����ȯ! ���� �� ������ �ƴ��� �� üũ�ؾ� �Ѵ�
				if (Enemy != nullptr) //���ʹ̰� �ƴϴ� = ���ʹ��̴�. �÷��̾� ��ġ�� ���� ���� ���ʹ̰� �ƴ� ���� �ֱ� ������ ���ʹ����� Ȯ�� �ʿ�
				{
					//���ʹ̰� ���, �̵�, ����, ���� ��� ���� �� ��
					if (Enemy->fsm->mState == EEnemyState::Idle || Enemy->fsm->mState == EEnemyState::Move || Enemy->fsm->mState == EEnemyState::Attack || Enemy->fsm->mState == EEnemyState::Pickup)
					{
						Enemy->fsm->OnDamageProcess(); //���ʹ� ������ �������� ����. ���ʹ�(���� ��� ����)���� fsm(���� ������Ʈ ��� ����)���� ���������μ��� ȣ��
					}
				}
			}
		}
	}
}

//[[3]BeginOverlap �÷��̾� �����ָ� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionPunchRBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains(TEXT("Enemy"))) //���࿡ �ε��� OtherActor �߿� �̸��� Enemy �� �����ϰ�������
	{
		isInputPunchGrab = true;
		currEnemy = OtherActor; //�ε��� OtherActor(�̸� �߿� Enemy �� �����ϰ� �ִ�)�� SH_Enemy Ŭ������ Enemy �̸� currEnemy ��� �Ѵ�
	}
}

//[[3]BeginOverlap �÷��̾� ���ָ� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionPunchLBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains(TEXT("Enemy"))) //���࿡ �ε��� OtherActor �߿� �̸��� Enemy �� �����ϰ�������
	{
		isInputPunchGrab = true;
		currEnemy = OtherActor; //�ε��� OtherActor(�̸� �߿� Enemy �� �����ϰ� �ִ�)�� SH_Enemy Ŭ������ Enemy �̸� currEnemy ��� �Ѵ�
	}
}

//[[3]EndOverlap �÷��̾� �����ָ� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionPunchREndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputPunchGrab = false;
}

//[[3]EndOverlap �÷��̾� ���ָ� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionPunchLEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputPunchGrab = false;
}


//[[3]BeginOverlap �÷��̾ �� �Ѹ��� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisonLollipopBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains(TEXT("Enemy"))) //���࿡ �ε��� OtherActor �߿� �̸��� Enemy �� �����ϰ�������
	{
		isInputPunchGrab = true;
		currEnemy = OtherActor; //�ε��� OtherActor(�̸� �߿� Enemy �� �����ϰ� �ִ�)�� SH_Enemy Ŭ������ Enemy �̸� currEnemy ��� �Ѵ�
	}
}

//[[3]EndOverlap �÷��̾ �� �Ѹ��� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisonLollipopEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputPunchGrab = false;
}



//############### ű ###############
// [���ű/������ �̺�Ʈ ó�� �Լ� ����]
void ARIM_Player::InputKickToss()
{
	if (isplayerDown == false) //�÷��̾ ������ ��
	{
		//�÷��̾� �ִϸ��̼� ��Ÿ�� �� 'ű' �ִϸ��̼� ���
		animPlayer->PlayPlayerTwoAnim(TEXT("Kick"), 0);

		UE_LOG(LogTemp, Error, TEXT("Player Kick!")); //Ȯ�ο� �ؽ�Ʈ ���

		if(isInputKickToss == true)
		{
			UE_LOG(LogTemp, Error, TEXT("Player Kick Attack!")); //Ȯ�ο� �ؽ�Ʈ ���

			Enemy = Cast<ASH_Enemy>(currEnemy); //���ʹ� ����ȯ! ���� �� ������ �ƴ��� �� üũ�ؾ� �Ѵ�
			if (Enemy != nullptr) //���ʹ̰� �ƴϴ� = ���ʹ��̴�. �÷��̾� ��ġ�� ���� ���� ���ʹ̰� �ƴ� ���� �ֱ� ������ ���ʹ����� Ȯ�� �ʿ�
			{
				//���ʹ̰� ���, �̵�, ����, ���� ��� ���� �� ��
				if (Enemy->fsm->mState == EEnemyState::Idle || Enemy->fsm->mState == EEnemyState::Move || Enemy->fsm->mState == EEnemyState::Attack || Enemy->fsm->mState == EEnemyState::Pickup)
				{
					Enemy->fsm->OnDamageProcess(); //���ʹ� ������ �������� ����. ���ʹ�(���� ��� ����)���� fsm(���� ������Ʈ ��� ����)���� ���������μ��� ȣ��
				}
			}
		}
	}
}

//[[3]BeginOverlap �÷��̾� ű �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionKickBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains(TEXT("Enemy"))) //���࿡ �ε��� OtherActor �߿� �̸��� Enemy �� �����ϰ�������
	{
		isInputKickToss = true;
		currEnemy = OtherActor; //�ε��� OtherActor(�̸� �߿� Enemy �� �����ϰ� �ִ�)�� SH_Enemy Ŭ������ Enemy �̸� currEnemy ��� �Ѵ�
	}
}

//[[3]EndOverlap �÷��̾� ű �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionKickEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputKickToss = false; 
}



//############### ��ġ�� ###############
// [��ġ�� �̺�Ʈ ó�� �Լ� ����]
void ARIM_Player::InputHeadbutt() //��� Ű ������ ��� �Ѵ�(�ִϸ��̼�) > 
{
	if (isplayerDown == false) //�÷��̾ ������ ��
	{
		//�÷��̾� �ִϸ��̼� ��Ÿ�� �� '���' �ִϸ��̼� ���
		animPlayer->PlayPlayerAnim(TEXT("Headbutt"), 0);

		UE_LOG(LogTemp, Error, TEXT("Player Headbutt!")); //Ȯ�ο� �ؽ�Ʈ ���

		if (isInputHeadbutt == true)
		{
			UE_LOG(LogTemp, Error, TEXT("Player Headbutt Attack!")); //Ȯ�ο� �ؽ�Ʈ ���

			Enemy = Cast<ASH_Enemy>(currEnemy); //���ʹ� ����ȯ! ���� �� ������ �ƴ��� �� üũ�ؾ� �Ѵ�
			if (Enemy != nullptr) //���ʹ̰� �ƴϴ� = ���ʹ��̴�. �÷��̾� ��ġ�� ���� ���� ���ʹ̰� �ƴ� ���� �ֱ� ������ ���ʹ����� Ȯ�� �ʿ�
			{
				//���ʹ̰� ���, �̵�, ����, ���� ��� ���� �� ��
				if (Enemy->fsm->mState == EEnemyState::Idle || Enemy->fsm->mState == EEnemyState::Move || Enemy->fsm->mState == EEnemyState::Attack || Enemy->fsm->mState == EEnemyState::Pickup)
				{
					Enemy->fsm->OnDamageProcess(); //���ʹ� ������ �������� ����. ���ʹ�(���� ��� ����)���� fsm(���� ������Ʈ ��� ����)���� ���������μ��� ȣ��
				}
			}
		}
	}
}

//[[3]BeginOverlap �÷��̾� ��ġ�� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionHeadbuttBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains(TEXT("Enemy"))) //���࿡ �ε��� OtherActor �߿� �̸��� Enemy �� �����ϰ�������
	{
		isInputHeadbutt = true;
		currEnemy = OtherActor; //�ε��� OtherActor(�̸� �߿� Enemy �� �����ϰ� �ִ�)�� SH_Enemy Ŭ������ Enemy �̸� currEnemy ��� �Ѵ�
	}
}

//[[3]EndOverlap �÷��̾� ��ġ�� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionHeadbuttEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputHeadbutt = false;
}



//[�÷��̾ ���� ���̰� �ϴ� �Լ� ����]
void ARIM_Player::VisibleGun()
{
	compMeshGun->SetVisibility(true); //�÷��̾ �� ���� ���̰� �Ѵ�
	//�ٴ��� ���� �ݸ����� ������ �ٴ��� ���Ⱑ �ı����� �ʴ´�? �ٴ��� ���� �ݸ����� �����Ѵ�? �ٴ��� ���� �ݸ����� �������� ���Ѵ�?
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0)); //�ڡڡ� ��ư(Ű) ����...? �𸣰���. �ñ���

	isPlayerVisibleGun = true;

}

//[�÷��̾ �Ѹ����� ���̰� �ϴ� �Լ� ����]
void ARIM_Player::VisibleLollipop()
{
	compMeshLollipop->SetVisibility(true); //�÷��̾ �� �Ѹ����� ���̰� �Ѵ�
	//�ٴ��� ���� �ݸ����� ������ �ٴ��� ���Ⱑ �ı����� �ʴ´�? �ٴ��� ���� �ݸ����� �����Ѵ�? �ٴ��� ���� �ݸ����� �������� ���Ѵ�?
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0)); //�ڡڡ� ��ư(Ű) ����...? �𸣰���. �ñ���

	isPlayerVisibleLollipop = true;

}

//[��, �Ѹ��� ���� ??? �Լ� ����] //�ڡڡ� �� �𸣰���. �� ������ �ұ�?
void ARIM_Player::EnableInput(APlayerController* PlayerController)
{
	Super::EnableInput(PlayerController);

	//[����/��� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��] = �Ѿ� �߻�. Weapon�� ���ε� �Ѱ�ٰ� �÷��̾ ������
	PlayerController->InputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, &ARIM_Player::InputPunchGrab);
}



//############### ���� �ٴڿ� ������ ###############
// [���� ������ �̺�Ʈ ó�� �Լ� ����]
void ARIM_Player::InputDropWeapon()
{	
	AItemSpawn* ItemSpawn = Cast<AItemSpawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AItemSpawn::StaticClass()));
		if (isPlayerVisibleGun == true) //���� �÷��̾��� ���� ���̸�(�÷��̾ ���� ��� ���� ��)
		{
			compMeshGun->SetVisibility(false); //�÷��̾��� �� ������Ʈ�� �Ⱥ��̰��Ѵ�
			//GetWorld()->SpawnActor<AGunWeapon>(weaponGun, GetActorLocation() + FVector(-200,0,0), GetActorRotation()); //GunWeapon�� �÷��̾� ��ġ�� �ٴڿ� �����ȴ�
			ItemSpawn->CreateWeapon(int32(EWeaponType::Gun), GetActorLocation() + FVector(0, 50, 50), GetActorRotation());
			UE_LOG(LogTemp, Error, TEXT("Player Gun Drop!")); //Ȯ�ο� �ؽ�Ʈ ���

			isPlayerVisibleGun = false;
		}
		else if (isPlayerVisibleLollipop == true) //���� �÷��̾��� �Ѹ����� ���̸�(�÷��̾ �Ѹ����� ��� ���� ��)
		{	
			compMeshLollipop->SetVisibility(false); //�÷��̾��� �� ������Ʈ�� �Ⱥ��̰��Ѵ�
			//GetWorld()->SpawnActor<ALollipopWeapon>(weaponLollipop, GetActorLocation() + FVector(-200, 0, 0), GetActorRotation()); //LollipopWeapon�� �÷��̾� ��ġ�� �ٴڿ� �����ȴ�
			ItemSpawn->CreateWeapon(int32(EWeaponType::Lollipop), GetActorLocation() + FVector(0, 50, 50), GetActorRotation());
			UE_LOG(LogTemp, Error, TEXT("Player Lollipop Drop!")); //Ȯ�ο� �ؽ�Ʈ ���

			isPlayerVisibleLollipop = false;
		}
}

		

//############### �÷��̾� HP, ������ ###############
//[�÷��̾� HP ���� �Լ�] �÷��̾ ���� ���ϸ� ���ʹ̿��� ȣ���ϴ� �Լ�. �÷��̾��� �������ϱ� �÷��̾�� ����
void ARIM_Player::OnDamageProcess()
{
	HP--; //�÷��̾� HP 1�� ����
	UE_LOG(LogTemp, Error, TEXT("%d"),HP); //Ȯ�ο� �ؽ�Ʈ ���
}

//[�÷��̾� ������, �˴ٿ� �Լ�] �÷��̾ ���� ���ϸ� ���ʹ̿��� ȣ���ϴ� �Լ�. �÷��̾��� �������ϱ� �÷��̾�� ����
void ARIM_Player::DamagePlay()
{
	if (isplayerDown == false) //������ ��
	{
		if (HP > 0)
		{
			//�÷��̾� �ִϸ��̼� ��Ÿ�� �� '������' �ִϸ��̼� ���� ���
			rand = FMath::RandRange(0, 2);
			animPlayer->PlayPlayerTwoAnim(TEXT("Damaged"), rand);

			InputDropWeapon();
		}
		else if (HP <= 0) //���� ���ʹ����� 5�� ���� ������(HP�� 5�̰� ���� ���������� HP�� 1�� �پ��ϱ� 5�� ���� �޴� ���� HP�� 0�� �Ǵ� �Ͱ� �����ϴ�)
		{
			//�÷��̾� �ִϸ��̼� ��Ÿ�� �� '�˴ٿ�' �ִϸ��̼� ���� ���
			rand = FMath::RandRange(0, 1);
			animPlayer->PlayPlayerTwoAnim(TEXT("Knockdown"), rand);

			UE_LOG(LogTemp, Error, TEXT("Player Knockdown!")); //Ȯ�ο� �ؽ�Ʈ ���

			//�˴ٿ� �� �� ���� ������ �ڡڡ�inputDropWeapon �� F ��ư ������ �� ���� ������ �ǵ�, ��ư �� ������ �������� �ؾ��ϴµ� ���� �� �ʿ��� ���̴� ����...
			InputDropWeapon();

			//�÷��̾� ����. ���� ��
			isplayerDown = true;

			//5�� �� �Ͼ�� �ڵ�� Tick���� �����Ѵ�. �ð��� �귯�� �ϴϱ�
		}
	}
}

//[�÷��̾� ����]
void ARIM_Player::Die()
{
		//�÷��̾� �ִϸ��̼� ��Ÿ�� �� '����' �ִϸ��̼� ���� ���
		rand = FMath::RandRange(0, 1);
		animPlayer->PlayPlayerTwoAnim(TEXT("Die"), rand);
		
		ADogFightGameModeBase* gamemodefail = GetWorld()->GetAuthGameMode<ADogFightGameModeBase>(); //�����´�
		gamemodefail->addtoViewfail(); //�Լ� ȣ��

		isplayerDown = true; //�÷��̾� ������. Ű ������ �� �� �����̰� �ϱ� ���� �߰� ��
		isplayerDie = true; //�÷��̾� �׾���
}



//int a = 10;
//int* b = &a;
//*b = 20;