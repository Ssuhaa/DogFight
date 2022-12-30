// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Bullet.h"
#include <Components/SphereComponent.h> //USphereComponent �� ����ϱ� ���� �߰�
#include <GameFramework/ProjectileMovementComponent.h> //UProjectileMovementComponentf �� ����ϱ� ���� �߰�

// Sets default values
ARIM_Bullet::ARIM_Bullet() //������
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[�Ѿ� �浹ü ������Ʈ �߰�]
	//�浹ü ����ϱ�. �浹ü �ν��Ͻ� ����ϰ� compCollision ������ ���
	//�浹ü�� ��Ʈ ������Ʈ�� ���
	//�浹ü ũ�� ����
	//�浹�������� ����. �浹ü�� �浹 ���������� BlockAll�� ����. ��� ��ü�� �ε��� ƨ�� �� �ְ� �ϱ�
	compCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = compCollision;
	compCollision->SetSphereRadius(13); //�ڡڡ����� ���� �ʿ�
	compCollision->SetCollisionProfileName(TEXT("BlockAll"));
	


	//[�Ѿ� ����ƽ�޽� ������Ʈ �߰�]
	//�ܰ� ������Ʈ ����ϱ�. UStaticMeshComponent �ν��Ͻ� ����� compMeshWeaponBullet ��� ������ �Ҵ�
	//�θ� ������Ʈ ����. �θ� ������Ʈ�� coolisionComp ����
	//�ܰ� ũ�� ����
	//�浹 ��Ȱ��ȭ
	compMeshWeaponBullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponBulletMeshComp"));
	compMeshWeaponBullet->SetupAttachment(compCollision);
	compMeshWeaponBullet->SetRelativeScale3D(FVector(0.25f)); //�ڡڡ����� ���� �ʿ�. �������Ʈ���� ���� �� ����
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
	compMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	compMovement->SetUpdatedComponent(compCollision);
	


	//[�Ѿ� �߻�ü ������Ʈ �ʱⰪ ����] //�ڡڡ���ü �ʿ� ���� �� ����
	//�ʱ�ӵ�. InitialSpeed �Ӽ� �̿�
	//�ִ�ӵ�. MaxSpeed �Ӽ� �̿�
	//�ݵ�����. bShouldBounce �Ӽ� �̿�. true �Ҵ�
	//�ݵ� ��. �ݵ��� �ִٸ� ź���� ��� ���� ���� Bounciness �Ӽ� �̿�
	//���� �ð� �ֱ�
	compMovement->InitialSpeed = 1000; //�ڡڡ����� ���� �ʿ�
	compMovement->MaxSpeed = 1000; //�ڡڡ����� ���� �ʿ�
	compMovement->bShouldBounce = true;
	compMovement->Bounciness = 0.8f; //�ڡڡ����� ���� �ʿ�
	//InitialLifeSpan = 10.0f; //�ڡڡ����� ���� �ʿ�
}

// Called when the game starts or when spawned
void ARIM_Bullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARIM_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

