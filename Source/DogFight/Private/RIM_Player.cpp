// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Player.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "RIM_Bullet.h" //★★★오류해결. Bullet.h 파일 소스를 열수없다고 한다 ---> 내 파일명 RIM_Bullet.h 으로 해야 함

// Sets default values
ARIM_Player::ARIM_Player() //생성자
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[스켈레탈메시. 옷 추가]
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor2.Charactor2'")); //★★★임시로 캐릭터2 넣음. 추후 필요시 변경
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object); 
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
		//gunMeshComp->SetVisibility(false); //★★★확인 필요. 디폴트로 총 안 보이게 하는 코드 이거 아닌가??? 컴파일은 되는데, 엔진 꺼짐. 실행 안됨
	}



	//[스프링암 컴포넌트 추가/USpringArmComponent 추가]
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90)); //★★★추후 필요시 변경
	springArmComp->TargetArmLength = 400; //★★★추후 필요시 변경
	springArmComp->bUsePawnControlRotation = true; //입력에 따른 회전 설정

	//[카메라 컴포넌트 추가/UCameraComponent 추가]
	playerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("playerCamComp"));
	playerCamComp->SetupAttachment(springArmComp);
	playerCamComp->bUsePawnControlRotation = false; //입력에 따른 회전 설정

	bUseControllerRotationYaw = true; //입력에 따른 회전 설정



	//[총 스켈레탈메시 컴포넌트 등록] //★★★책에 스켈레탈메시로 만들어서 동일하게 했는데, 총인데 왜 스켈레탈메시로 해야하는지 궁금
	//총 스켈레탈메시 컴포넌트 등록
	//부모 컴포넌트를 Mesh 컴포넌트로 설정
	//스켈레탈메시 데이터 로드
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh(),TEXT("Gun")); //총을 캐릭터 소켓에 붙임
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	//[총 스켈레탈메시 컴포넌트 데이터 설정]
	//스켈레탈메시 데이터 로드가 성공했다면
	//스켈레탈메시 데이터 할당
	//위치 조정하기
	if (TempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(-90, 0, -159)); //★★★총 위치. 임시로 세팅. 추후 변경 필요
		gunMeshComp->SetRelativeScale3D(FVector(0.5f)); //★★★총 크기. 임시로 세팅. 추후 변경 필요
	}

}

// Called when the game starts or when spawned
void ARIM_Player::BeginPlay()
{
	Super::BeginPlay();

	//[초기 속도를 걷기로 설정]
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	gunMeshComp->SetVisibility(false);
	
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

	//AddMovementInput() 함수를 이용하여 위에 있는 등속운동 코드 대체
	direction = FTransform(GetControlRotation()).TransformVector(direction); //이동 방향을 컨트롤 방향 기준으로 변환
	AddMovementInput(direction); //
	direction = FVector::ZeroVector;
}


// Called to bind functionality to input
void ARIM_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//[좌우 회전 이벤트 처리 함수 바인딩/호출]
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ARIM_Player::Turn);

	//[좌우 입력 이벤트 처리 함수 바인딩/호출]
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ARIM_Player::InputHorizontal);

	//[상하 입력 이벤트 처리 함수 바인딩/호출]
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ARIM_Player::InputVertical);

	//[점프 입력 이벤트 처리 함수 바인딩/호출]
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ARIM_Player::InputJump);

	//[달리기 입력 이벤트 처리 함수 바인딩/호출]
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &ARIM_Player::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &ARIM_Player::InputRun);

	//[공격/잡기 이벤트 처리 함수 바인딩/호출] = 총알 발사
	PlayerInputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, &ARIM_Player::InputPunchGrab);

	//[드롭킥/던지기 이벤트 처리 함수 바인딩/호출]

	//[무기 버리기 이벤트 처리 함수 바인딩/호출]
	
	//[벽타기 이벤트 처리 함수 바인딩/호출]
	
	//[박치기 이벤트 처리 함수 바인딩/호출]

	//[구르기 이벤트 처리 함수 바인딩/호출]

	//[달리기 점프 이벤트 처리 함수 바인딩/호출] ★★★구현 안 해도 됨
	//[달리기 공격 이벤트 처리 함수 바인딩/호출] ★★★아마 구현 안 해도 됨

}

//[좌우 회전 함수 구현]
void ARIM_Player::Turn(float value)
{
	AddControllerYawInput(value);
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


//[공격/잡기 이벤트 처리 함수 구현] = 총알 발사
void ARIM_Player::InputPunchGrab()
{
	//FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition")); //★★★책 내용인데 지금 필요 없을 것 같음
	//GetWorld()->SpawnActor<ARIM_Bullet>(bulletFactory, firePosition); //★★★책 내용인데 지금 필요 없을 것 같음
	if (gunMeshComp->IsVisible() == true)
	{
	GetWorld()->SpawnActor<ARIM_Bullet>(bulletFactory, GetActorLocation(), GetActorRotation());
	UE_LOG(LogTemp, Warning, TEXT("Attack!")); //★★★나중에 삭제

	}
}



//[드롭킥/던지기 이벤트 처리 함수 구현]

//[무기 버리기 이벤트 처리 함수 구현]

//[벽타기 이벤트 처리 함수 구현]

//[박치기 이벤트 처리 함수 구현]

//[구르기 이벤트 처리 함수 구현]


//[점프 달리기 이벤트 처리 함수 구현] ★★★구현 안 해도 됨
//[달리기 공격 이벤트 처리 함수 구현] ★★★아마 구현 안 해도 됨


//[플레이어가 들고 있는 총이 안 보였다 보이는 함수 구현]
void ARIM_Player::VisibleGun()
{
	gunMeshComp->SetVisibility(true);
}



void ARIM_Player::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(LogTemp , Warning, TEXT("4444444444"));
}