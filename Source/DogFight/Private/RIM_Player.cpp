// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Player.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

// Sets default values
ARIM_Player::ARIM_Player() //������
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[���̷�Ż�޽�]
	// ConstructorHelpers::FObjectFinder = ����ü �ڡڡڱ���ü�� �������� Ȯ��
	// USkeletalMesh �ڡڡھ𸮾����� ������ �ִ� Ŭ������� �����ϸ� �Ǵ��� Ȯ��
	// TempMesh �ڡڡں���...?
	// �ּ� ĳ����2 ���� ��� �߰�
	// TempMesh(ĳ����2 ������) �ε� �����ϸ�,
	// �޽�->���̷�Ż�޽ÿ� TempMesh(ĳ����2) �߰� �ڡڡ����� �� ��
	// �޽�->���̷�Ż�޽��� ��ġ��, ȸ���� ����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor2.Charactor2'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object); 
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0)); 
	}

	//[�������� ������Ʈ �߰�]
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;

	//[ī�޶� ������Ʈ �߰�]
	playerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("playerCamComp"));
	playerCamComp->SetupAttachment(springArmComp);

}

// Called when the game starts or when spawned
void ARIM_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARIM_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARIM_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

