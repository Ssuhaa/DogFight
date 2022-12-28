// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Bullet.h"
#include <Components/SphereComponent.h> //USphereComponent �� ����ϱ� ���� �߰�
#include <GameFramework/ProjectileMovementComponent.h> //UProjectileMovementComponentf �� ����ϱ� ���� �߰�

// Sets default values
ARIM_Bullet::ARIM_Bullet() //������
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[�浹ü ������Ʈ �߰�]
	//�浹ü ����ϱ�. �浹ü �ν��Ͻ� ����ϰ� collisionComp ������ ���
	//�浹ü�� ��Ʈ ������Ʈ�� ���
	//�浹ü ũ�� ����
	//�浹�������� ����. �浹ü�� �浹 ���������� BlockAll�� ����. ��� ��ü�� �ε��� ƨ�� �� �ְ� �ϱ�
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = collisionComp;
	collisionComp->SetSphereRadius(13); //�ڡڡ����� ���� �ʿ�
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	


	//[����ƽ�޽� ������Ʈ �߰�]
	//�ܰ� ������Ʈ ����ϱ�. UStaticMeshComponent �ν��Ͻ� ����� bodyMeshComp ��� ������ �Ҵ�
	//�θ� ������Ʈ ����. �θ� ������Ʈ�� coolisionComp ����
	//�ܰ� ũ�� ����
	//�浹 ��Ȱ��ȭ
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	bodyMeshComp->SetupAttachment(collisionComp);
	bodyMeshComp->SetRelativeScale3D(FVector(0.25f)); //�ڡڡ����� ���� �ʿ�. �������Ʈ���� ���� �� ����
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);



	//[�߻�ü ������Ʈ �߰�]
	//�߻�ü ������Ʈ
	//movement ������Ʈ�� �����ų ������Ʈ ����
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	movementComp->SetUpdatedComponent(collisionComp);
	


	//[�߻�ü ������Ʈ �ʱⰪ ����] //�ڡڡ���ü �ʿ� ���� �� ����
	//�ʱ�ӵ�. InitialSpeed �Ӽ� �̿�
	//�ִ�ӵ�. MaxSpeed �Ӽ� �̿�
	//�ݵ�����. bShouldBounce �Ӽ� �̿�. true �Ҵ�
	//�ݵ� ��. �ݵ��� �ִٸ� ź���� ��� ���� ���� Bounciness �Ӽ� �̿�
	//���� �ð� �ֱ�
	movementComp->InitialSpeed = 1000; //�ڡڡ����� ���� �ʿ�
	movementComp->MaxSpeed = 1000; //�ڡڡ����� ���� �ʿ�
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.8f; //�ڡڡ����� ���� �ʿ�
	InitialLifeSpan = 10.0f; //�ڡڡ����� ���� �ʿ�
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

