// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Enemy.h"
#include "SH_EnemyFSM.h"
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values
ASH_Enemy::ASH_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor1.Charactor1'")); //�޽� ������ ã��
	if (TempMesh.Succeeded())//���� �޽������͸� ã�Ҵٸ�
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object); // ���ʹ� �޽������� ����
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0)); // ���ʹ� ��ġ�� ȸ���� ����
	}

	fsm = CreateDefaultSubobject<USH_EnemyFSM>(TEXT("FSM")); //	���ʹ̿� FSM������Ʈ �߰�

	
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void ASH_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASH_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

// Called to bind functionality to input
void ASH_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

