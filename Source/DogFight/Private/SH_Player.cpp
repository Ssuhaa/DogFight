// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Player.h"
#include "GameFramework/SpringArmComponent.h"
#include <Camera/CameraComponent.h>
#include "SH_EnemyFSM.h"
#include <Components/BoxComponent.h>


// Sets default values
ASH_Player::ASH_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//스켈레탈 메시 설정
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor2.Charactor2'")); //캐릭터 메쉬 경로를 찾아 TempMesh에 저장한다.
	if (TempMesh.Succeeded()) //만약 TempMesh를 찾았다면
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object); // 캐릭터에 메쉬를 넣는다.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0)); //캐릭터 메쉬의 위치값과 회전값을 새로 세팅한다.
	}
	//스프링암 컴포넌트
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp")); //SpringArm 컴포넌트 생성
	springArmComp->SetupAttachment(RootComponent); // 루트 컴포넌트에 자식으로 붙이기
	springArmComp->SetRelativeLocation((FVector(0, 70, 90))); // 위치 수정
	springArmComp->TargetArmLength = 400; // 타깃과 카메라 사이에 충돌이 없을 경우 타깃과의 지정 거리
	springArmComp->bUsePawnControlRotation = true; // 회전값이 들어오면 스프링암 컴포넌트를 회전시킬지 여부
	//카메라 컴포넌트
	camComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camComp->SetupAttachment(springArmComp); // 스프링암 컴포넌트에 카메라를 자식으로 붙임
	camComp->bUsePawnControlRotation = false; //회전값이 들어오면  카메라를 회전시킬지 여부 
	
	bUseControllerRotationYaw = true; //사용자의 입력에 따라  캐릭터를 회전시킬지 여부

	JumpMaxCount = 2; //플레이어가 2단 점프가 가능하게 만들기

	
}

// Called when the game starts or when spawned
void ASH_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASH_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(); // 플레이어 이동 함수

}

// Called to bind functionality to input
void ASH_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ASH_Player::Turn); //프로젝트 입력 축 매핑 Turn에 Turn 함수 바인딩
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ASH_Player::LookUp); // 프로젝트 입력 축 매핑 LockUp에 LockUp 함수 바인딩
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ASH_Player::InputHorizontal); //프로젝트 입력 축매핑 Horizontal에 InputHorizontal 함수 바인딩
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ASH_Player::InputVertical); // 프로젝트 입력 축 매핑 Vertical에 InputVertical 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ASH_Player::InputJump); //프로젝트 입력 축 매핑 Jump에 Jump 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, & ASH_Player::inputAttack); //프로젝트 입력축 매핑 Attack에 Attack 함수 바인딩
}

void ASH_Player::Move()
{
	//플레이어 이동구현
	direction = FTransform(GetControlRotation()).TransformVector(direction);//이동방향을 컨트롤방향 기준으로 전환 . 이 코드가 없으면 월드방향 기준 
	/*
	FVector P0 = GetActorLocation(); //현재위치
	FVector vt = direction * walkSpeed * DeltaTime; //가려고하는 위치 * 이동속도 * 시간
	FVector P = P0 + vt; //미래위치
	SetActorLocation(P); // 플레이어 위치 설정
	*///위 코드는 AddMovementInput() 함수로 대체 가능
	AddMovementInput(direction);
	direction = FVector::ZeroVector; // direction 초기화
}

void ASH_Player::Turn(float value)//좌우 회전입력처리 함수
{
	AddControllerYawInput(value);
}
void ASH_Player::LookUp(float value) //상하 회전입력처리 함수
{
	AddControllerPitchInput(value);
}
void ASH_Player::InputHorizontal(float value)// 좌우이동 이벤트 입력처리 함수
{
	direction.Y = value;
}
void ASH_Player:: InputVertical(float value)// 상하 이동 이벤트 입력처리 함수
{
	direction.X = value;
}
void ASH_Player::InputJump() //점프 이벤트 입력처리
{
	Jump();
}

void ASH_Player::inputAttack() //공격 이벤트 입력처리
{

	//라인트레이스 준비
	FVector startPos = camComp->GetComponentLocation(); //라인트레이스 처리 camComp를 다른 컴포넌트로 교체해야할 것으로 예상
	FVector endPos = camComp->GetComponentLocation() + camComp->GetForwardVector() * 5000;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

	if (bHit) //만약 히트된것이 있고
	{
		//부딛힌 대상이 적인지 판단하기
		auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM")); 
		if (enemy)
		{
			auto enemyFSM = Cast<USH_EnemyFSM>(enemy);//에너미FSM형변환
			enemyFSM->OnDamageProcess(); //데미지 함수 호출
			UE_LOG(LogTemp, Warning, TEXT("PlayerAttack!")); //로그 출력
		}
	}

}
