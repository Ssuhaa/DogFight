// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Bullet.h"
#include <Components/SphereComponent.h> //USphereComponent �� ����ϱ� ���� �߰�
#include <GameFramework/ProjectileMovementComponent.h> //UProjectileMovementComponentf �� ����ϱ� ���� �߰�
#include "SH_Enemy.h"
#include "SH_EnemyFSM.h"

// Sets default values
ARIM_Bullet::ARIM_Bullet() //������
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[�Ѿ� �浹ü ������Ʈ �߰�]
	//�浹ü ����ϱ�. �浹ü �ν��Ͻ� ����ϰ� compCollision ������ ���
	//�浹ü�� ��Ʈ ������Ʈ�� ���
	//�浹ü ũ�� ����
	//�浹�������� ����. �浹ü�� �浹 ��������/�������� Weapon ���� ����
	compCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = compCollision;
	compCollision->SetSphereRadius(40); //������ �ʿ�� ����
	compCollision->SetCollisionProfileName(TEXT("Weapon"));
	


	//[�Ѿ� ����ƽ�޽� ������Ʈ �߰�]
	//�ܰ� ������Ʈ ����ϱ�. UStaticMeshComponent �ν��Ͻ� ����� compMeshWeaponBullet ��� ������ �Ҵ�
	//�θ� ������Ʈ ����. �θ� ������Ʈ�� coolisionComp ����
	//�ܰ� ũ�� ����
	//�浹 ��Ȱ��ȭ
	compMeshWeaponBullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponBulletMeshComp"));
	compMeshWeaponBullet->SetupAttachment(compCollision);
	compMeshWeaponBullet->SetRelativeLocation(FVector(10.260604f, 0, -28.190779f));
	compMeshWeaponBullet->SetRelativeScale3D(FVector(0.25f)); //������ �ʿ�� ����. �������Ʈ���� ���� �� ����
	compMeshWeaponBullet->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//[�Ѿ� ����ƽ�޽�. �� �߰�]
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (tempMesh.Succeeded())
	{
		compMeshWeaponBullet->SetStaticMesh(tempMesh.Object);
	}



	//[�Ѿ� �߻�ü ������Ʈ �߰�]
	//�߻�ü ������Ʈ
	//movement ������Ʈ�� �����ų ������Ʈ ����
	compMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp")); //�ڡڡ�???
	compMovement->SetUpdatedComponent(compCollision); //�ڡڡ�???
	


	//[�Ѿ� �߻�ü ������Ʈ �ʱⰪ ����] //�ڡڡ���ü �ʿ� ���� �� ����
	//�ʱ�ӵ�. InitialSpeed �Ӽ� �̿�
	//�ִ�ӵ�. MaxSpeed �Ӽ� �̿�
	//�ݵ�����. bShouldBounce �Ӽ� �̿�. true �Ҵ�
	//�ݵ� ��. �ݵ��� �ִٸ� ź���� ��� ���� ���� Bounciness �Ӽ� �̿�
	//���� �ð� �ֱ�
	compMovement->InitialSpeed = 2000; //������ �ʿ�� ����
	compMovement->MaxSpeed = 5000; //������ �ʿ�� ����
	compMovement->bShouldBounce = true;
	compMovement->Bounciness = 0.2f; //������ �ʿ�� ����
	InitialLifeSpan = 1.0f; //������ �ʿ�� ����
}

// Called when the game starts or when spawned
void ARIM_Bullet::BeginPlay()
{
	Super::BeginPlay();

	//[[1]�Ѿ� �ݸ����� ���ʹ̰� �浹 �� ���� �� �Լ� ȣ��] �ڡڡڿϺ��ϰ� �������� �� ��. �� BeginPlay���� �ϴ���???
	compCollision->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Bullet::collisionBulletBeginOverlap);


}

// Called every frame
void ARIM_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//[[3]BeginOverlap �Ѿ� �ݸ����� ���ʹ̰� �浹�� �� ������ ���� / �Լ� ����)]
void ARIM_Bullet::collisionBulletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains(TEXT("Enemy"))) //���࿡ �ε��� OtherActor �߿� �̸��� Enemy �� �����ϰ�������
	{
		UE_LOG(LogTemp, Error, TEXT("Player Fire Attack!!!")); //Ȯ�ο� �ؽ�Ʈ ���

		//[���ʹ̿� �������� ���� �Լ� ȣ��]
		Enemy = Cast<ASH_Enemy>(OtherActor); //���ʹ� ����ȯ! ���� �� ������ �ƴ��� �� üũ�ؾ� �Ѵ�
		                                     //�ε��� OtherActor(�̸� �߿� Enemy �� �����ϰ� �ִ�)�� SH_Enemy Ŭ������ Enemy �̸� Enemy ��� �Ѵ�
		
		if (Enemy != nullptr) //���ʹ̰� �ƴϴ� = ���ʹ��̴�. �÷��̾� �Ѿ˿� ���� ���� ���ʹ̰� �ƴ� ���� �ֱ� ������ ���ʹ����� Ȯ�� �ʿ�
		{
			//���ʹ̰� ���, �̵�, ����, ���� ��� ���� �� ��
			if (Enemy->fsm->mState == EEnemyState::Idle || Enemy->fsm->mState == EEnemyState::Move || Enemy->fsm->mState == EEnemyState::Attack || Enemy->fsm->mState == EEnemyState::Pickup)
			{
				Enemy->fsm->OnDamageProcess(); //SH_Enemy Ŭ���� Enemy(���� ��� ����)���� fsm(���� ������Ʈ ��� ����)���� ���������μ��� ȣ��
				Destroy(); //�Ѿ� �ݸ����� ���ʹ̿� �浹�ϸ� �ı� �ȴ�
			}
		}
	}
}