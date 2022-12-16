// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Player.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

// Sets default values
ARIM_Player::ARIM_Player() //생성자
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[스켈레탈메시]
	// ConstructorHelpers::FObjectFinder = 구조체 ★★★구조체란 무엇인지 확인
	// USkeletalMesh ★★★언리얼엔진이 가지고 있는 클래스라고 이해하면 되는지 확인
	// TempMesh ★★★변수...?
	// 애셋 캐릭터2 파일 경로 추가
	// TempMesh(캐릭터2 데이터) 로드 성공하면,
	// 메시->스켈레탈메시에 TempMesh(캐릭터2) 추가 ★★★이해 안 됨
	// 메시->스켈레탈메시의 위치값, 회전값 변경
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor2.Charactor2'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object); 
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0)); 
	}

	//[스프링암 컴포넌트 추가]
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;

	//[카메라 컴포넌트 추가]
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

