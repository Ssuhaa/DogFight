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
	compSpringArm->SetupAttachment(RootComponent);
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


	
	//[�� ������Ʈ �߰�] --->���̷�Ż ����
	compMeshGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMeshComp"));
	compMeshGun->SetupAttachment(GetMesh(), TEXT("Gun")); //�θ� ������Ʈ�� Mesh ������Ʈ�� ����. ���� ĳ���� �� ���Ͽ� ����...?
	
	//[�� ���̷�Ż�޽� �߰�. �� �߰�]
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempGunMesh(TEXT("StaticMesh'/Game/Geometry/Gun.Gun'"));
	if (tempGunMesh.Succeeded())
	{
		compMeshGun->SetStaticMesh(tempGunMesh.Object);
		compMeshGun->SetRelativeLocation(FVector(5.759851f, -0.573986f, 26.993545f)); //�ڡڡ��� ��ġ. �ӽ÷� ����. ���� ���� �ʿ�
		compMeshGun->SetRelativeRotation(FRotator(0, -159, - 90)); //�ڡڡ��� ȸ��. �ӽ÷� ����. ���� ���� �ʿ�
		compMeshGun->SetRelativeScale3D(FVector(0.5f)); //�ڡڡ��� ũ��. �ӽ÷� ����. ���� ���� �ʿ�
	}
	
	//[�Ѹ��� ������Ʈ �߰�] --->����ƽ ����
	//compMeshLollipop->CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LollipopMeshComp"));
	//compMeshLollipop->SetupAttachment(GetMesh(), TEXT("Lollipop")); //�θ� ������Ʈ�� Mesh ������Ʈ�� ����. �Ѹ����� ĳ���� �� ���Ͽ� ����...?

	//[�Ѹ��� ����ƽ�޽� �߰�. �� �߰�]
	//ConstructorHelpers::FObjectFinder<UStaticMesh> tempLollipopMesh(TEXT("StaticMesh'/Game/Geometry/Lollipop.Lollipop'"));
	//if (tempLollipopMesh.Succeeded())
	//{
	//	compMeshLollipop->SetStaticMesh(tempLollipopMesh.Object);
	//}



	//[�÷��̾� ��ġ �浹 ������Ʈ �߰�]
	compCollisionPunchR = CreateDefaultSubobject<UBoxComponent>(TEXT("PunchMeshCompR"));
	compCollisionPunchL = CreateDefaultSubobject<UBoxComponent>(TEXT("PunchMeshCompL"));
	compCollisionPunchR->SetRelativeScale3D(FVector(0.5f));
	compCollisionPunchL->SetRelativeScale3D(FVector(0.5f));
	compCollisionPunchR->SetupAttachment(GetMesh(), TEXT("Prop_RSocket")); //������. �浹 ������Ʈ�� ĳ���� ���Ͽ� ����
	compCollisionPunchL->SetupAttachment(GetMesh(), TEXT("Prop_LSocket")); //�޼�. �浹 ������Ʈ�� ĳ���� ���Ͽ� ����

// 	//[�÷��̾� ű �浹 ������Ʈ �߰�]
// 	compCollisionKick = CreateDefaultSubobject<UBoxComponent>(TEXT("KickMeshComp"));
// 	//compCollisionKick->SetRelativeLocation(FVector());
// 	//compCollisionKick->SetRelativeScale3D(FVector(0.5f));
// 	compCollisionKick->SetupAttachment(GetMesh(), TEXT("ball_lSocket"));
// 
// 	//[�÷��̾� ��� �浹 ������Ʈ �߰�]
// 	compCollisionHeadbutt = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadbuttMechComp"));
// 	//compCollisionHeadbutt->SetRelativeScale3D(FVector(0.5f));
// 	compCollisionHeadbutt->SetupAttachment(GetMesh(), TEXT("headSocket"));


	//�ҷ����丮 �ȿ� �ҷ� ����Ʈ�� �߰� �ڡڡڿ� �� �۾��� �ʿ����� �ñ�
	ConstructorHelpers::FClassFinder<ARIM_Bullet> tempBul(TEXT("Blueprint'/Game/BluePrint/BP_RIM_Bullet.BP_RIM_Bullet_C'")); //����. C++ ���� ã�� �� ���ٰ��Ͽ� �������Ʈ ���Ϸ� ����
	if(tempBul.Succeeded())
	{
		bulletFactory = tempBul.Class;
	}

	//�ִϸ��̼�...?
// 	ConstructorHelpers::FClassFinder<URIM_PlayerAnim> tempAnim(TEXT("AnimBlueprint'/Game/BluePrint/ABP_RIM_PlayerAnim.ABP_RIM_PlayerAnim_C'"));
// 	if (tempAnim.Succeeded())
// 	{
// 		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
// 	}

	//���� ������ ����
	ConstructorHelpers::FClassFinder<ARIM_Gun> TempGun(TEXT("Blueprint'/Game/BluePrint/BP_RIM_Gun.BP_RIM_Gun_c'"));
	if (TempGun.Succeeded())
	{
		Gun = TempGun.Class;
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

	//[�Ѹ����� ����Ʈ�� �� ���̰� ����]
	//compMeshLollipop->SetVisibility(false);


	//[[1]�÷��̾� �ָ� �ݸ����� ���ʹ̰� �浹 �� ���� �� �Լ� ȣ��]
	//��ġ �ݸ��� ������Ʈ ������ ��� �Լ� OnComponentBeginOverlap�� OnComponentEndOverlap ��������Ʈ ȣ�� + ��ġ �ݸ��� ������Ʈ�� �ؿ� ������ �Լ� ����
	compCollisionPunchR->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionPunchRBeginOverlap);
	compCollisionPunchL->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionPunchLBeginOverlap);
	compCollisionPunchR->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionPunchREndOverlap);
	compCollisionPunchL->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionPunchLEndOverlap);

	//[1]�÷��̾� ű �ݸ����� ���ʹ̰� �⵹ �� ���� �� �Լ� ȣ��
// 	compCollisionKick->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionKickBeginOverlap);
// 	compCollisionKick->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionKickEndOverlap);

	//[1]�÷��̾� ��ġ�� �ݸ����� ���ʹ̰� �浹 �� ���� �� �Լ� ȣ��
// 	compCollisionHeadbutt->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionHeadbuttBeginOverlap);
// 	compCollisionHeadbutt->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionHeadbuttEndOverlap);



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

	//[���� ������ �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]
	PlayerInputComponent->BindAction(TEXT("DropWeapon"), IE_Pressed, this, &ARIM_Player::InputDropWeapon);
	
	//[���ű/������ �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]
	//PlayerInputComponent->BindAction(TEXT("KickToss"), IE_Pressed, this, &ARIM_Player::InputKickToss);

	
	//[��ġ�� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��]
	//PlayerInputComponent->BindAction(TEXT("Headbutt"), IE_Pressed, this, &ARIM_Player::InputHeadbutt);



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
	else //���� �÷��̾ ���� ��� ���� ���� ��
	{
		if (isInputPunchGrab == true) //isInputPunchGrab Ű�� ������
		{
			//��ġ �ִϸ��̼��� �÷����Ѵ�

			ASH_Enemy* Enemy = Cast<ASH_Enemy>(currEnemy); //���ʹ� ����ȯ
			Enemy->fsm->OnDamageProcess(); //���ʹ�(���� ��� ����)���� fsm(���� ������Ʈ ��� ����)���� ���������μ��� ȣ��
		}
	}
}



//[���� ������ �̺�Ʈ ó�� �Լ� ����]
void ARIM_Player::InputDropWeapon()
{
	//���̹���, ����ƽ���� 2�� �ʿ�
	
	//�÷��̾ ������ �ִ� ���� ������Ʈ�� �Ⱥ��̰��Ѵ�
	if (compMeshGun->IsVisible())
	{
		GetWorld()->SpawnActor<ARIM_Gun>(Gun, GetActorLocation(), GetActorRotation());	
	}
	compMeshGun->SetVisibility(false);
	//���Ⱑ �÷��̾� ��ġ(�ٴ�)�� �����ȴ�
}



//[�÷��̾ ��� �ִ� ���� �� ������ ���̴� �Լ� ����]
void ARIM_Player::VisibleGun()
{
	//GetMesh()->SetSkeletalMesh(��ȣ�ۿ���)

	compMeshGun->SetVisibility(true);
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

}



// [���ű/������ �̺�Ʈ ó�� �Լ� ����]
// void ARIM_Player::InputKickToss()
// {
// 	//���ű ���ϸ��̼��� �÷����Ѵ�
//  
// 	ASH_Enemy* Enemy = Cast<ASH_Enemy>(currEnemy); //���ʹ� ����ȯ
// 	Enemy->fsm->OnDamageProcess(); //���ʹ�(���� ��� ����)���� fsm(���� ������Ʈ ��� ����)���� ���������μ��� ȣ��
// }



// [��ġ�� �̺�Ʈ ó�� �Լ� ����]
// void ARIM_Player::InputHeadbutt()
// {
// 	//��ġ�� �ִϸ��̼��� �÷����Ѵ�.
// 
// 	ASH_Enemy* Enemy = Cast<ASH_Enemy>(currEnemy); //���ʹ� ����ȯ
// 	Enemy->fsm->OnDamageProcess(); //���ʹ�(���� ��� ����)���� fsm(���� ������Ʈ ��� ����)���� ���������μ��� ȣ��
// }



//[???]
void ARIM_Player::EnableInput(APlayerController* PlayerController)
{
	Super::EnableInput(PlayerController);

	//[����/��� �̺�Ʈ ó�� �Լ� ���ε�/ȣ��] = �Ѿ� �߻�. Gun�� ���ε� �Ѱ�ٰ� �÷��̾ ������
	PlayerController->InputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, &ARIM_Player::InputPunchGrab);
}



//[�÷��̾� �ָ� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionPunchRBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//���࿡ �ε��� OtherActor�� �̸��� enemy�� �����ϰ�������
	if (OtherActor->GetName().Contains(TEXT("Enemy")))
	{

		isInputPunchGrab = true;
		currEnemy = OtherActor;
	}
}

//[�÷��̾� �ָ� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionPunchLBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//���࿡ �ε��� OtherActor�� �̸��� enemy�� �����ϰ�������
	if (OtherActor->GetName().Contains(TEXT("Enemy")))
	{
		isInputPunchGrab = true;
		currEnemy = OtherActor;
	}
}

//[�÷��̾� �ָ� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionPunchREndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputPunchGrab = false;
}

//[�÷��̾� �ָ� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
void ARIM_Player::collisionPunchLEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputPunchGrab = false;
}



//[�÷��̾� ű �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
// void ARIM_Player::collisionKickBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	//���࿡ �ε��� OtherActor�� �̸��� enemy�� �����ϰ�������
// 	if (OtherActor->GetName().Contains(TEXT("Enemy")))
// 	{
// 		isInputKickToss = true;
// 		currEnemy = OtherActor;
// 
// 	}
// }

//[�÷��̾� ű �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
// void ARIM_Player::collisionKickEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// 	isInputKickToss = false;
// }



//[�÷��̾� ��ġ�� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
// void ARIM_Player::collisionHeadbuttBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	//���࿡ �ε��� OtherActor�� �̸��� enemy�� �����ϰ�������
// 	if (OtherActor->GetName().Contains(TEXT("Enemy")))
// 	{
// 		isInputHeadbutt = true;
// 		currEnemy = OtherActor;
//  
// 	}
// }

//[�÷��̾� ��ġ�� �ݸ����� ���ʹ̰� �浹 �� ������ ����/�Լ� ����]
// void ARIM_Player::collisionHeadbuttEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// 	isInputHeadbutt = false;
// }



//[�÷��̾ ���� ���ϸ� ���ʹ̿��� ȣ���ϴ� �Լ�] �÷��̾��� �������ϱ� �÷��̾�� ����
void ARIM_Player::OnDamageProcess()
{
	UE_LOG(LogTemp, Warning, TEXT("Damaged!!!")); //�ӽ÷� �ؽ�Ʈ ���
	
	if (HP > 0)
	{
		HP--; //�÷��̾� HP 1�� ����
	}
	else //�÷��̾ ���ʹ����� 5�� ���� ������(HP�� 5�̰� ���� ���������� HP�� 1�� �پ��ϱ� 5�� ���� �޴� ���� HP�� 0�� �Ǵ� �Ͱ� �����ϴ�)
	{
		UE_LOG(LogTemp, Warning, TEXT("I'll be back!")); //�÷��̾ �����Ѵ�(�ִϸ��̼�). �ӽ÷� �ؽ�Ʈ ���
		
		//�����ϸ� ����� �ʵ�

		currentTime += GetWorld()->GetDeltaSeconds(); //����ð� + ��ŸŸ�� = ���� �����ð�
		if (currentTime > 5) //�÷��̾ ������ �ð�(5��)�� ������
		{	
			UE_LOG(LogTemp, Warning, TEXT("Hello!")); //�÷��̾ �Ͼ��(�ִϸ��̼�). �ӽ÷� �ؽ�Ʈ ���
			currentTime = 0; //���� �ð��� 0�ʷ� �ʱ�ȭ �Ѵ�
			HP = 5; //�÷��̾��� HP�� 5�ʷ� �ʱ�ȭ �Ѵ�
		}
	}
}