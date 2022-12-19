// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Bow.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ARIM_Bow::ARIM_Bow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	//[Ȱ ���̷�Ż�޽� ������Ʈ ���]
	//Ȱ ���̷�Ż�޽� ������Ʈ ���
	//�θ� ������Ʈ�� Mesh ������Ʈ�� ����
	//���̷�Ż�޽� ������ �ε�
	bowMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BowMeshComp"));
	bowMeshComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempBowMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor2.Charactor2'")); //�ڡڡ��ӽ÷� �۸��� ����

	//[Ȱ ���̷�Ż�޽� ������Ʈ ������ ����]
	//���̷�Ż�޽� ������ �ε尡 �����ߴٸ�
	//���̷�Ż�޽� ������ �Ҵ�
	//��ġ �����ϱ�

	if (TempBowMesh.Succeeded())
	{
		bowMeshComp->SetSkeletalMesh(TempBowMesh.Object);
		bowMeshComp->SetRelativeLocation(FVector(0, 50, 120)); //�ڡڡ��ӽ÷� ����. ���� ���� �ʿ�
	}
	*/
}

// Called when the game starts or when spawned
void ARIM_Bow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARIM_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

