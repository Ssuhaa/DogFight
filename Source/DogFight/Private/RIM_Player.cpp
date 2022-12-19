// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Player.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values
ARIM_Player::ARIM_Player() //생성자
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[스켈레탈메시. 캐릭터 추가]
	//애셋 캐릭터2 파일 경로 추가
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor2.Charactor2'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object); 
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0)); 
	}



	//[스프링암 컴포넌트 추가/USpringArmComponent 추가]
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;

	//[카메라 컴포넌트 추가/UCameraComponent 추가]
	playerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("playerCamComp"));
	playerCamComp->SetupAttachment(springArmComp);
}

// Called when the game starts or when spawned
void ARIM_Player::BeginPlay()
{
	Super::BeginPlay();

	//[초기 속도를 걷기로 설정]
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	
}

// Called every frame
void ARIM_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//[Move(플레이어 이동) 함수 호출]
	Move();

}

//[Move(플레이어 이동) 함수 구현?]
void ARIM_Player::Move()
{
	//[플레이어 이동 구현 코드]

	//P(결과 위치) = P0(현재 위치) + v(속도=크기*방향) * t(시간)
	//direction = FTransform(GetControlRotation()). TransformVector(direction);
	//FVector P0 = GetActorLocation();
	//FVector vt = walkSpeed * direction * DeltaTime; //v=walkSpeed * direction, t=DeltaTime
	//FVector P = P0 + vt;
	//SetActorLocation(P);
	//direction = FVector::ZeroVector; //방향 direction의 모든 요소(x, y, z)에 0을 항당하여 초기화

	//AddMovementInput() 함수를 이용하여 등속운동 코드 대체
	direction = FTransform(GetControlRotation()).TransformVector(direction); //이동 방향을 컨트롤 방향 기준으로 변환
	AddMovementInput(direction); //
	direction = FVector::ZeroVector;
}


// Called to bind functionality to input
void ARIM_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//[좌우 입력 이벤트 처리 함수 바인딩/호출]
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ARIM_Player::InputHorizontal);

	//[상하 입력 이벤트 처리 함수 바인딩/호출]
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ARIM_Player::InputVertical);

	//[점프 입력 이벤트 처리 함수 바인딩/호출]
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ARIM_Player::InputJump);

	//[달리기 입력 이벤트 처리 함수 바인딩/호출]
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &ARIM_Player::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &ARIM_Player::InputRun);

	//[달리기 점프 이벤트 처리 함수 바인딩/호출] ★★★구현 안 해도 됨

	//[공격/잡기 이벤트 처리 함수 바인딩/호출]

	//[달리기 공격 이벤트 처리 함수 바인딩/호출] ★★★아마 구현 안 해도 됨

	//[벽타기 이벤트 처리 함수 바인딩/호출]

	//[드롭킥/던지기 이벤트 처리 함수 바인딩/호출]

	//[박치기 이벤트 처리 함수 바인딩/호출]

	//[구르기 이벤트 처리 함수 바인딩/호출]

	//[무기 버리기 이벤트 처리 함수 바인딩/호출]


	/*
	//[총알 발사 이벤트 처리 함수 바인딩/호출]
	//InputPunchGrab 함수 바인딩/호출
	PlayerInputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, & ARIM_Player::InputPunchGrab);
	*/
}

//[좌우 입력 함수 구현]
void ARIM_Player::InputHorizontal(float value)
{
	direction.Y = value;
}

//[상하 입력 함수 구현]
void ARIM_Player::InputVertical(float value)
{
	direction.X = value;
}

//[점프 입력 함수 구현]
void ARIM_Player::InputJump()
{
	Jump();
}

//[달리기 입력 함수 구현]
void ARIM_Player::InputRun()
{
	auto movement = GetCharacterMovement();

	if (movement->MaxWalkSpeed > walkSpeed) //현재 달리기 모드라면(초기값이 걷기 모드이므로 walkSeed보다 크지 않음)
	{
		movement->MaxWalkSpeed = walkSpeed; //걷기 속도로 전환
	}
	else
	{
		movement->MaxWalkSpeed = runSpeed; //달리기 속도로 전환
	}
}

//[점프 달리기 이벤트 처리 함수 구현] ★★★구현 안 해도 됨

//[공격/잡기 이벤트 처리 함수 구현]
//펀치 공격 한다.
//캐릭터를 잡는다.
//무기를 잡는다. 한 번 무기를 잡으면 버튼을 클릭 안 해도 무기를 들고 있는다. 
//무기를 잡은 상태에서, 다시 L Mauseu Button 클릭하면 잡은 무기를 휘두른다.
//무기가 총/활 일 때 다시 L Mauseu Button 클릭하면 잡은 무기에서 총알/화살이 발사된다. 

//[달리기 공격 이벤트 처리 함수 구현] ★★★아마 구현 안 해도 됨

//[벽타기 이벤트 처리 함수 구현]

//[드롭킥/던지기 이벤트 처리 함수 구현]

//[박치기 이벤트 처리 함수 구현]

//[구르기 이벤트 처리 함수 구현]

//[무기 버리기 이벤트 처리 함수 구현]



/*
#include "RIM_Bullet.h" //★★★오류. Bullet.h 파일 소스를 열수없다고 한다 ---> 내 파일명 RIM_Bullet.h 으로 해야 함
void ARIM_Player::InputPunchGrab()
{
	FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ARIM_Bullet>(bulletFactory, firePosition);
}
*/