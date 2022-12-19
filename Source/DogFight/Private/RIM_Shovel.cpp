// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Shovel.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/SphereComponent.h>

// Sets default values
ARIM_Shovel::ARIM_Shovel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	//[�� ���̷�Ż�޽� ������Ʈ ���]
	//�� ���̷�Ż�޽� ������Ʈ ���
	//�θ� ������Ʈ�� Mesh ������Ʈ�� ����
	//���̷�Ż�޽� ������ �ε�
	shovelMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShovelMeshComp"));
	shovelMeshComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempShovelMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor1.Charactor1'")); //�ڡڡ��ӽ÷� �䳢 ����

	//[�� ���̷�Ż�޽� ������Ʈ ������ ����]
	//���̷�Ż�޽� ������ �ε尡 �����ߴٸ�
	//���̷�Ż�޽� ������ �Ҵ�
	//��ġ �����ϱ�

	if (TempShovelMesh.Succeeded())
	{
		shovelMeshComp->SetSkeletalMesh(TempShovelMesh.Object);
		shovelMeshComp->SetRelativeLocation(FVector(-14, 52, 120)); //�ڡڡ��ӽ÷� ����. ���� ���� �ʿ�
	}

	//[�浹ü ������Ʈ �߰�] �ڡڡ����� �Ѿ˰� ������ ������ ���� ��. ���� ���� �ʿ�
	//�浹ü ����ϱ�. �浹ü �ν��Ͻ� ����ϰ� collisionComp ������ ���
	//�浹�������� ����. �浹ü�� �浹 ���������� BlockAll�� ����. ��� ��ü�� �ε��� ƨ�� �� �ְ� �ϱ�
	//�浹ü ũ�� ����
	//�浹ü�� ��Ʈ ������Ʈ�� ���

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	collisionComp->SetSphereRadius(13);
	RootComponent = collisionComp;
	*/
}

// Called when the game starts or when spawned
void ARIM_Shovel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARIM_Shovel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

