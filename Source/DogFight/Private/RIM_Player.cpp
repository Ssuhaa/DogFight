// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Player.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Components/BoxComponent.h>
#include "RIM_Bullet.h"
#include "SH_Enemy.h"
#include "SH_EnemyFSM.h"

// Sets default values
ARIM_Player::ARIM_Player() //생성자
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[플레이어 스켈레탈메시. 옷 추가]
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor2.Charactor2'")); //★★★임시로 캐릭터2 넣음. 추후 필요시 변경
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object); 
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	//[스프링암 컴포넌트 추가/USpringArmComponent 추가]
	compSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	compSpringArm->SetupAttachment(RootComponent);
	compSpringArm->SetRelativeLocation(FVector(0, 70, 90)); //★★★추후 필요시 변경
	compSpringArm->TargetArmLength = 400; //★★★추후 필요시 변경
	compSpringArm->bUsePawnControlRotation = true; //입력에 따른 회전 설정

	//[카메라 컴포넌트 추가/UCameraComponent 추가]
	compPlayerCam = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamComp"));
	compPlayerCam->SetupAttachment(compSpringArm);
	compPlayerCam->bUsePawnControlRotation = false; //입력에 따른 회전 설정

	bUseControllerRotationYaw = true; //입력에 따른 회전 설정


	//[총 스켈레탈메시 컴포넌트 등록]
	//스켈레탈메시 데이터 로드
	//부모 컴포넌트를 Mesh 컴포넌트로 설정
	compMeshGun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	compMeshGun->SetupAttachment(GetMesh(), TEXT("Gun")); //총을 캐릭터 소켓에 붙임
	
	//[총 스켈레탈메시. 옷 추가]
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (TempGunMesh.Succeeded())
	{
		compMeshGun->SetSkeletalMesh(TempGunMesh.Object);
		compMeshGun->SetRelativeLocation(FVector(5.759851f, -0.573986f, 26.993545f)); //★★★총 위치. 임시로 세팅. 추후 변경 필요
		compMeshGun->SetRelativeRotation(FRotator(0, -159, - 90)); //★★★총 회전. 임시로 세팅. 추후 변경 필요
		compMeshGun->SetRelativeScale3D(FVector(0.5f)); //★★★총 크기. 임시로 세팅. 추후 변경 필요
	}


	//[플레이어 펀치 충돌 컴포넌트 등록]
	//충돌 컴포넌트 추가
	//충돌 컴포넌트를 캐릭터에 붙임
	compCollisionPunchR = CreateDefaultSubobject<UBoxComponent>(TEXT("PunchMeshCompR"));
	compCollisionPunchL = CreateDefaultSubobject<UBoxComponent>(TEXT("PunchMeshCompL"));
	compCollisionPunchR->SetRelativeScale3D(FVector(0.5f));
	compCollisionPunchL->SetRelativeScale3D(FVector(0.5f));
	compCollisionPunchR->SetupAttachment(GetMesh(), TEXT("Prop_RSocket")); //오른손
	compCollisionPunchL->SetupAttachment(GetMesh(), TEXT("Prop_LSocket")); //왼손
	
//불렛팩토리안에 불렛디폴트로 추가한것
	ConstructorHelpers::FClassFinder<ARIM_Bullet> tempBul(TEXT("Blueprint'/Game/BluePrint/BP_RIM_Bullet.BP_RIM_Bullet_C'"));
	if(tempBul.Succeeded())
	{
		bulletFactory = tempBul.Class;
	}

}

// Called when the game starts or when spawned
void ARIM_Player::BeginPlay()
{
	Super::BeginPlay();

	//[초기 속도를 걷기로 설정]
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	
	//[총이 디폴트로 안보이게 설정]
	compMeshGun->SetVisibility(false);

	//[플레이어 주먹과 충돌 시... ???]
	compCollisionPunchR->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionPunchRBeginOverlap);
	compCollisionPunchL->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionPunchLBeginOverlap);
	compCollisionPunchR->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionPunchREndOverlap);
	compCollisionPunchL->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionPunchLEndOverlap);


	
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

	//[공격/잡기 이벤트 처리 함수 바인딩/호출] = 총알 발사 + 펀치
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


//[공격/잡기 이벤트 처리 함수 구현] = 총알 발사 + 펀치
void ARIM_Player::InputPunchGrab()
{	
	//만약 플레이어가 총을 들고 있으면
	//총의 총구에서 총알을 발사한다.
	if (compMeshGun->IsVisible() == true)
	{
		FTransform trFire = compMeshGun->GetSocketTransform(TEXT("FirPos")); //총 스켈레톤에서 소켓 추가
		GetWorld()->SpawnActor<ARIM_Bullet>(bulletFactory, trFire); //월드에서 ARIM_Bullet(총알) 가져온다

		//GetWorld()->SpawnActor<ARIM_Bullet>(bulletFactory, GetActorLocation() + GetActorForwardVector() * 200, GetActorRotation());

	}
	else
	{
		if (isInputPunchGrab == true)
		{
			ASH_Enemy* Enemy = Cast<class ASH_Enemy>(currEnemy); //에너미 형변환
			Enemy->fsm->OnDamageProcess(); //에너미(액터 상속 받음)에서 fsm(액터 컴포넌트 상속 받음)에서 데미지프로세스 호출

			//펀치 애니메이션을 플레이한다.

		}
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
	compMeshGun->SetVisibility(true);
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ARIM_Player::EnableInput(APlayerController* PlayerController)
{
	Super::EnableInput(PlayerController);

	//[공격/잡기 이벤트 처리 함수 바인딩/호출] = 총알 발사. Gun에 바인딩 넘겼다가 플레이어가 가져옴
	PlayerController->InputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, &ARIM_Player::InputPunchGrab);
}



//[플레이어 주먹과 충돌 시 함수 구현]
void ARIM_Player::collisionPunchRBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//만약에 부딛힌 OtherActor의 이름이 enemy를 포함하고있으면
	if (OtherActor->GetName().Contains(TEXT("Enemy")))  //GetDefaultSubojectByname
	{
		isInputPunchGrab = true;
		currEnemy = OtherActor;
	}
}

void ARIM_Player::collisionPunchLBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//만약에 부딛힌 OtherActor의 이름이 enemy를 포함하고있으면
	if (OtherActor->GetName().Contains(TEXT("Enemy")))
	{
		isInputPunchGrab = true;
		currEnemy = OtherActor;
	}
}

void ARIM_Player::collisionPunchREndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputPunchGrab = false;
}

void ARIM_Player::collisionPunchLEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputPunchGrab = false;
}



//[플레이어가 공격 당하면 에너미에서 호출]
void ARIM_Player::OnDamageProcess()
{
	//HP 감소
	//만약 플레이어의 HP가 0보다 같거나 적으면 플레이어를 파괴한다
	HP--; 
	if (HP <= 0)
	{
		Destroy();
	}
}
