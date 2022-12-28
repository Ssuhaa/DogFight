// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Gun.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
ARIM_Gun::ARIM_Gun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[�浹ü ������Ʈ �߰�]
	//�浹ü ����ϱ�. �浹ü �ν��Ͻ� ����ϰ� collisionComp ������ ���
	//�浹ü�� ��Ʈ ������Ʈ�� ���
	//�浹�������� ����. �浹ü�� �浹 ���������� BlockAll�� ����. ��� ��ü�� �ε��� ƨ�� �� �ְ� �ϱ�
	collisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = collisionComp;
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));



	//[����ƽ�޽� ������Ʈ �߰�]
	//�ܰ� ������Ʈ ����ϱ�. UStaticMeshComponent �ν��Ͻ� ����� bodyMeshComp ��� ������ �Ҵ�
	//�θ� ������Ʈ ����. �θ� ������Ʈ�� coolisionComp ����
	//�浹 ��Ȱ��ȭ
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("itemGunMeshComp"));
	bodyMeshComp->SetupAttachment(collisionComp);
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	


	//[movementComp ������Ʈ �߰�] //�ڡڡ��ʿ� ���� �� ��ü ����
	//movementComp ������Ʈ
	//movement ������Ʈ�� �����ų ������Ʈ ����
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	movementComp->SetUpdatedComponent(collisionComp);



	//[movementComp ������Ʈ �ʱⰪ ����] //�ڡڡ��ʿ� ���� �� ��ü ����
	//�ʱ�ӵ�. InitialSpeed �Ӽ� �̿�
	//�ִ�ӵ�. MaxSpeed �Ӽ� �̿�
	//�ݵ�����. bShouldBounce �Ӽ� �̿�. true �Ҵ�
	//�ݵ� ��. �ݵ��� �ִٸ� ź���� ��� ���� ���� Bounciness �Ӽ� �̿�
	//���� �ð� �ֱ�
	movementComp->InitialSpeed = 500; //�ڡڡ����� ���� �ʿ�
	movementComp->MaxSpeed = 500; //�ڡڡ����� ���� �ʿ�
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.2f; //�ڡڡ����� ���� �ʿ�
	InitialLifeSpan = 30.0f; //�ڡڡ����� ���� �ʿ�


}

// Called when the game starts or when spawned
void ARIM_Gun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARIM_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}