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



	//[�������� ������Ʈ �߰�. USpringArmComponent]
	compSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	compSpringArm->SetupAttachment(RootComponent); //��Ʈ������Ʈ�� �ڽ����� ����
	compSpringArm->SetRelativeLocation(FVector(0, 0, 0)); //�ڡڡ����� �ʿ�� ����
	compSpringArm->SetRelativeRotation(FRotator(0, 0, 0)); //�ڡڡ����� �ʿ�� ����
	compSpringArm->TargetArmLength = 500; //�ڡڡ����� �ʿ�� ����
	compSpringArm->SocketOffset = FVector(0, 0, 380); //�ڡڡ����� �ʿ�� ����
	compSpringArm->bUsePawnControlRotation = true; //�Է¿� ���� ȸ�� ����

	//[ī�޶� ������Ʈ �߰�. UCameraComponent]
	compPlayerCam = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamComp"));
	compPlayerCam->SetupAttachment(compSpringArm);
	compPlayerCam->SetRelativeLocation(FVector(-15, 0, 12));
	compPlayerCam->SetRelativeRotation(FRotator(-35, 0, 0)); //�ڡڡ����� �ʿ�� ����
	compPlayerCam->bUsePawnControlRotation = false; //�Է¿� ���� ȸ�� ����

	bUseControllerRotationYaw = true; //�Է¿� ���� ȸ�� ����

	
	//[�� ������Ʈ �߰�]
	compMeshGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMeshComp"));
	compMeshGun->SetupAttachment(GetMesh(), TEXT("Gun")); //�θ� ������Ʈ�� Mesh ������Ʈ�� ����. ���� ĳ���� �� ���Ͽ� ����...?
		
	//[�� ����ƽ�޽� �߰�. �� �߰�] 
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempGunMesh(TEXT("StaticMesh'/Game/Geometry/Gun.Gun'")); 
	if (tempGunMesh.Succeeded())
	{
		compMeshGun->SetStaticMesh(tempGunMesh.Object);
		compMeshGun->SetRelativeLocation(FVector(-8.280492f, 21.242059f, 5.475584f)); //�ڡڡ��� ��ġ. �ӽ÷� ����. ���� ���� �ʿ�
		compMeshGun->SetRelativeRotation(FRotator(60.000000f, -339.000000f, -270.000000f)); //�ڡڡ��� ȸ��. �ӽ÷� ����. ���� ���� �ʿ�
	}
	
	//[�Ѹ��� ����ƽ�޽� �߰�]
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



	//[�÷��̾� ��ġ �浹 ������Ʈ �߰�]
	compCollisionPunchR = CreateDefaultSubobject<UBoxComponent>(TEXT("PunchMeshCompR"));
	compCollisionPunchL = CreateDefaultSubobject<UBoxComponent>(TEXT("PunchMeshCompL"));
	compCollisionPunchR->SetRelativeScale3D(FVector(0.5f));
	compCollisionPunchL->SetRelativeScale3D(FVector(0.5f));
	compCollisionPunchR->SetupAttachment(GetMesh(), TEXT("Prop_RSocket")); //������. �浹 ������Ʈ�� ĳ���� ���Ͽ� ����
	compCollisionPunchL->SetupAttachment(GetMesh(), TEXT("Prop_LSocket")); //�޼�. �浹 ������Ʈ�� ĳ���� ���Ͽ� ����

 	//[�÷��̾� ű �浹 ������Ʈ �߰�]
 	compCollisionKick = CreateDefaultSubobject<UBoxComponent>(TEXT("KickMeshComp"));
	compCollisionKick->SetRelativeLocation(FVector(26, 10, 6));
	compCollisionKick->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
 	compCollisionKick->SetupAttachment(GetMesh(), TEXT("ball_lSocket")); //�޹�. �浹 ������Ʈ�� ĳ���� ���Ͽ� ����
 
	//[�÷��̾� ��� �浹 ������Ʈ �߰�]
 	compCollisionHeadbutt = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadbuttMechComp"));
	compCollisionHeadbutt->SetRelativeLocation(FVector(0, 29, 188));
 	compCollisionHeadbutt->SetRelativeScale3D(FVector(1.5f, 0.8f, 1));
 	compCollisionHeadbutt->SetupAttachment(GetMesh(), TEXT("headSocket")); //�Ӹ�. �浹 ������Ʈ�� ĳ���� ���Ͽ� ����


	//[�ҷ����丮 �ȿ� �ҷ� ����Ʈ�� �߰�] �ڡڡڿ� �� �۾��� �ʿ����� �ñ�
	ConstructorHelpers::FClassFinder<ARIM_Bullet> tempBul(TEXT("Blueprint'/Game/BluePrint/BP_RIM_Bullet.BP_RIM_Bullet_C'")); //����. C++ ���� ã�� �� ���ٰ��Ͽ� �������Ʈ ���Ϸ� ����
	if(tempBul.Succeeded())
	{
		bulletFactory = tempBul.Class;
	}

	//[�ִϸ��̼� ����]
	ConstructorHelpers::FClassFinder<URIM_PlayerAnim> tempAnim(TEXT("AnimBlueprint'/Game/BluePrint/ABP_Player.ABP_Player_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

// 	//���� ������ ����...?
// 	ConstructorHelpers::FClassFinder<AWeapon> tempWeapon(TEXT("StaticMesh'/Game/Geometry/Gun.Gun_C'"));
// 	if (tempWeapon.Succeeded())
// 	{
// 		weapon = tempWeapon.Class;
// 	}


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


	//[[1]�÷��̾� �ָ� �ݸ����� ���ʹ̰� �浹 �� ���� �� �Լ� ȣ��]
	//��ġ �ݸ��� ������Ʈ ������ ��� �Լ� OnComponentBeginOverlap�� OnComponentEndOverlap ��������Ʈ ȣ�� + ��ġ �ݸ��� ������Ʈ�� �ؿ� ������ �Լ� ����
	compCollisionPunchR->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionPunchRBeginOverlap);
	compCollisionPunchL->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionPunchLBeginOverlap);
	compCollisionPunchR->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionPunchREndOverlap);
	compCollisionPunchL->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionPunchLEndOverlap);

	//[1]�÷��̾� ű �ݸ����� ���ʹ̰� �⵹ �� ���� �� �Լ� ȣ��
	compCollisionKick->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionKickBeginOverlap);
	compCollisionKick->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionKickEndOverlap);

	//[1]�÷��̾� ��ġ�� �ݸ����� ���ʹ̰� �浹 �� ���� �� �Լ� ȣ��
	compCollisionHeadbutt->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionHeadbuttBeginOverlap);
	compCollisionHeadbutt->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionHeadbuttEndOverlap);



}

// Called every frame
void ARIM_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//[Move(�÷��̾� �̵�) �Լ� ȣ��]
	Move();


}

//[Move(�÷��̾� �̵�) �Լ� ����]
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
	PlayerInputComponent->BindAction(TEXT("KickToss"), IE_Pressed, this, &ARIM_Player::InputKickToss);
	
	//[��ġ�� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]
	PlayerInputComponent->BindAction(TEXT("Headbutt"), IE_Pressed, this, &ARIM_Player::InputHeadbutt);

	//[���� ������ �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]
	//PlayerInputComponent->BindAction(TEXT("DropWeapon"), IE_Pressed, this, &ARIM_Player::InputDropWeapon);


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


//############### ��ġ, �Ѿ� �߻� ###############
//[����/��� �̺�Ʈ ó�� �Լ� ����] = �Ѿ� �߻� + ��ġ
void ARIM_Player::InputPunchGrab()
{	
	if (compMeshGun->IsVisible() == true) //���� �÷��̾ ���� ��� ������(�÷��̾ �� ���� ���̸�) -> �ѿ��� �Ѿ��� �߻��Ѵ�
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire!!!")); //�ӽ÷� �ؽ�Ʈ ���

		//FTransform trFire = compMeshGun->GetSocketTransform(TEXT("FirPos")); //�� ���̷��濡�� ���� �߰� ---> ó���� �� ���̷���޽��� �� ��� �ߴ� �ڵ�
		//GetWorld()->SpawnActor<ARIM_Bullet>(bulletFactory, trFire); ////���� �ѱ����� �Ѿ��� �߻��Ѵ�. ���忡�� ARIM_Bullet(�Ѿ�) �����´� ---> ó���� �� ���̷���޽��� �� ��� �ߴ� �ڵ�

		GetWorld()->SpawnActor<ARIM_Bullet>(bulletFactory, GetActorLocation() + GetActorForwardVector() * 200, GetActorRotation());

// 		ASH_Enemy* Enemy = Cast<ASH_Enemy>(currEnemy);//���ʹ� ����ȯ ���� �� ������ �ƴ��� �� üũ�ؾ���
// 		if(Enemy != nullptr)
// 		{
// 			Enemy->fsm->OnDamageProcess(); //���ʹ�(���� ��� ����)���� fsm(���� ������Ʈ ��� ����)���� ���������μ��� ȣ�� -----> �Ѿ˿��� ������ ������!!!�ڡڡڡڡڡڡڡ�
// 		}
	}
	else if (compMeshLollipop->IsVisible() == true) //�÷��̾ ���� ��� ���� �ʰ�, �Ѹ����� ��� ������(�÷��̾ �� �Ѹ����� ���̸�) -> �Ѹ������� �����Ѵ�
	{	
		if (isInputPunchGrab == true) //�÷��̾ �Ѹ����� �� ���·� isInputPunchGrab Ű�� ������
		{
			UE_LOG(LogTemp, Warning, TEXT("Lollipop!!")); //�Ѹ��� �ִϸ��̼��� �÷����Ѵ�. �ӽ÷� �ؽ�Ʈ ���

			ASH_Enemy* Enemy = Cast<ASH_Enemy>(currEnemy); //���ʹ� ����ȯ
			Enemy->fsm->OnDamageProcess(); //���ʹ�(���� ��� ����)���� fsm(���� ������Ʈ ��� ����)���� ���������μ��� ȣ��
		}
	}
	else  //�÷��̾ ���� ��� ���� �ʰ�, �Ѹ����� ��� ���� �ʰ� InputPunchGrab ��ư�� ������ -> ��ġ�Ѵ� 
	{
		//���࿡ 
		if (isInputPunchGrab == true) //isInputPunchGrab Ű�� ������
		{
			UE_LOG(LogTemp, Warning, TEXT("Punch!!")); //�ӽ÷� �ؽ�Ʈ ���

			ASH_Enemy* Enemy = Cast<ASH_Enemy>(currEnemy); //���ʹ� ����ȯ
			Enemy->fsm->OnDamageProcess(); //���ʹ�(���� ��� ����)���� fsm(���� ������Ʈ ��� ����)���� ���������μ��� ȣ��

		}
		
		//�÷��̾� �ִϸ��̼� ��Ÿ�� �� '��ġ' �ִϸ��̼� ���

	}
	URIM_PlayerAnim* animPlayer = Cast<URIM_PlayerAnim>(GetMesh()->GetAnimInstance());
	animPlayer->PlayPlayerAnim(TEXT("Punch"), 0);

}

//[BeginOverlap �÷��̾� �����ָ� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionPunchRBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//���࿡ �ε��� OtherActor�� �̸��� enemy�� �����ϰ�������
	if (OtherActor->GetName().Contains(TEXT("Enemy")))
	{
		isInputPunchGrab = true;
		currEnemy = OtherActor; //�ε��� OtherActor �̸��� enemy�� �����ϰ� �ִ� ���͸� currEnemy��� ��
	}
}

//[BeginOverlap �÷��̾� ���ָ� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionPunchLBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//���࿡ �ε��� OtherActor�� �̸��� enemy�� �����ϰ�������
	if (OtherActor->GetName().Contains(TEXT("Enemy")))
	{
		isInputPunchGrab = true;
		currEnemy = OtherActor; //�ε��� OtherActor �̸��� enemy�� �����ϰ� �ִ� ���͸� currEnemy��� ��
	}
}

//[EndOverlap �÷��̾� �����ָ� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionPunchREndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputPunchGrab = false;
}

//[EndOverlap �÷��̾� ���ָ� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionPunchLEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputPunchGrab = false;
}



//[�÷��̾ ��� �ִ� ���� �� ������ ���̴� �Լ� ����]
void ARIM_Player::VisibleGun()
{
	compMeshGun->SetVisibility(true); //�÷��̾ �� ���� ���̰� �Ѵ�
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0)); //�ڡڡ� ��ư(Ű) ����...? �𸣰���. �ñ���
}

//[�÷��̾ ��� �ִ� �Ѹ����� �� ������ ������ �Լ� ����]
void ARIM_Player::VisibleLollipop()
{
	compMeshLollipop->SetVisibility(true); //�÷��̾ �� �Ѹ����� ���̰� �Ѵ�
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0)); //�ڡڡ� ��ư(Ű) ����...? �𸣰���. �ñ���
}


//[��, �Ѹ��� ���� ??? �Լ� ����] //�ڡڡ� �� �𸣰���. �� ������ �ұ�?
void ARIM_Player::EnableInput(APlayerController* PlayerController)
{
	Super::EnableInput(PlayerController);

	//[����/��� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��] = �Ѿ� �߻�. Gun�� ���ε� �Ѱ�ٰ� �÷��̾ ������
	PlayerController->InputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, &ARIM_Player::InputPunchGrab);
}



//############### ű ###############
// [���ű/������ �̺�Ʈ ó�� �Լ� ����]
void ARIM_Player::InputKickToss()
{
	if(isInputKickToss == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Kick!!!")); //���ű ���ϸ��̼��� �÷����Ѵ�. �ӽ÷� �ؽ�Ʈ ���

		ASH_Enemy* Enemy = Cast<ASH_Enemy>(currEnemy); //���ʹ� ����ȯ
		Enemy->fsm->OnDamageProcess(); //���ʹ�(���� ��� ����)���� fsm(���� ������Ʈ ��� ����)���� ���������μ��� ȣ��
	}
}

//[�÷��̾� ű �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionKickBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//���࿡ �ε��� OtherActor�� �̸��� enemy�� �����ϰ�������
	if (OtherActor->GetName().Contains(TEXT("Enemy")))
	{
		isInputKickToss = true;
		currEnemy = OtherActor; //�ε��� OtherActor �̸��� enemy�� �����ϰ� �ִ� ���͸� currEnemy��� ��
	}
}

//[�÷��̾� ű �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionKickEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputKickToss = false; 
}


//############### ��ġ�� ###############
// [��ġ�� �̺�Ʈ ó�� �Լ� ����]
void ARIM_Player::InputHeadbutt()
{
	if (isInputHeadbutt == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Headbutt!!!")); //��ġ�� �ִϸ��̼��� �÷����Ѵ�. �ӽ÷� �ؽ�Ʈ ���

		ASH_Enemy* Enemy = Cast<ASH_Enemy>(currEnemy); //���ʹ� ����ȯ
		Enemy->fsm->OnDamageProcess(); //���ʹ�(���� ��� ����)���� fsm(���� ������Ʈ ��� ����)���� ���������μ��� ȣ��
	}
}

//[�÷��̾� ��ġ�� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionHeadbuttBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//���࿡ �ε��� OtherActor�� �̸��� enemy�� �����ϰ�������
	if (OtherActor->GetName().Contains(TEXT("Enemy")))
	{
		isInputHeadbutt = true;
		currEnemy = OtherActor; //�ε��� OtherActor �̸��� enemy�� �����ϰ� �ִ� ���͸� currEnemy��� ��
	}
}

//[�÷��̾� ��ġ�� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionHeadbuttEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputHeadbutt = false;
}



//############### ���� �ٴڿ� ������ ###############
// [���� ������ �̺�Ʈ ó�� �Լ� ����]
// void ARIM_Player::InputDropWeapon()
// {
// 	//�÷��̾ ���� ��� ���� �� --->���� InputPunchGrab �Լ� �����ο� �߰��ؾ� �ϳ�?
// 	//InputDropWeapon Ű�� ������...? --->���� InputPunchGrab �Լ� �����ο� �߰��ؾ� �ϳ�?
// 	if (compMeshGun->SetVisibility(false)) //�÷��̾ ��� �ִ� �� ������Ʈ�� �Ⱥ��̰��Ѵ�
// 	{
// 		//���� �÷��̾� ��ġ(�ٴ�)�� �����ȴ�
// 		GetWorld()->SpawnActor<AGunWeapon>(weaponGun, GetActorLocation(), GetActorRotation());
// 	}
// 	//�÷��̾ �Ѹ����� ��� ���� �� --->���� InputPunchGrab �Լ� �����ο� �߰��ؾ� �ϳ�?
// 	//InputDropWeapon Ű�� ������...? --->���� InputPunchGrab �Լ� �����ο� �߰��ؾ� �ϳ�?
// 	else if(compMeshLollipop->SetVisibility(false)) //�÷��̾ ��� �ִ� �Ѹ��� ������Ʈ�� �Ⱥ��̰� �Ѵ�
// 	{
// 		//�Ѹ����� �÷��̾� ��ġ(�ٴ�)�� �����ȴ�
// 		GetWorld()->SpawnActor<ALollipopWeapon>(weaponLol, GetActorLocation(), GetActorRotation());
// 	}
// 	else
// 	{
// 		return false;
// 	}
// }
		

//############### �÷��̾� HP, ������ ###############
//[�÷��̾ ���� ���ϸ� ���ʹ̿��� ȣ���ϴ� �Լ�] �÷��̾��� �������ϱ� �÷��̾�� ����
void ARIM_Player::OnDamageProcess()
{
	UE_LOG(LogTemp, Warning, TEXT("Player Damaged!!!")); //�ӽ÷� �ؽ�Ʈ ���
	
	HP--; //�÷��̾� HP 1�� ����
	
	if (HP <= 0) //���� ���ʹ����� 5�� ���� ������(HP�� 5�̰� ���� ���������� HP�� 1�� �پ��ϱ� 5�� ���� �޴� ���� HP�� 0�� �Ǵ� �Ͱ� �����ϴ�)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player down!")); //�÷��̾ �����Ѵ�(�ִϸ��̼�). �ӽ÷� �ؽ�Ʈ ���

		//���ʹ��� �������� ���� �ʴ´�. HP�� �������� �ʴ´�?

		currentTime += GetWorld()->GetDeltaSeconds(); //����ð� + ��ŸŸ�� = ���� �����ð�
		if (currentTime > 5) //�÷��̾ ������ �ð�(5��)�� ������
		{	
			UE_LOG(LogTemp, Warning, TEXT("Player up!")); //�÷��̾ �Ͼ��(�ִϸ��̼�). �ӽ÷� �ؽ�Ʈ ���
			currentTime = 0; //���� �ð��� 0�ʷ� �ʱ�ȭ �Ѵ�
			HP = 5; //�÷��̾��� HP�� 5�ʷ� �ʱ�ȭ �Ѵ�
		}
	}
}

//int a = 10;
//int* b = &a;
//*b = 20;