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
#include "RIM_Gun.h"
#include "RIM_PlayerAnim.h"

// Sets default values
ARIM_Player::ARIM_Player() //생성자
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[플레이어 스켈레탈메시 추가. 옷 추가]
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor2.Charactor2'")); //옷 파일 엔진 폴더에 업로드
	if (tempMesh.Succeeded()) //파일 업로드 됐으면
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object); //캐릭터 메시의 스켈레탈메시에 파일 업로드. 옷 장착 
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}



	//[스프링암 컴포넌트 추가. USpringArmComponent]
	compSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	compSpringArm->SetupAttachment(RootComponent);
	compSpringArm->SetRelativeLocation(FVector(0, 0, 0)); //★★★추후 필요시 변경
	compSpringArm->SetRelativeRotation(FRotator(0, 0, 0)); //★★★추후 필요시 변경
	compSpringArm->TargetArmLength = 500; //★★★추후 필요시 변경
	compSpringArm->SocketOffset = FVector(0, 0, 380); //★★★추후 필요시 변경
	compSpringArm->bUsePawnControlRotation = true; //입력에 따른 회전 설정

	//[카메라 컴포넌트 추가. UCameraComponent]
	compPlayerCam = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamComp"));
	compPlayerCam->SetupAttachment(compSpringArm);
	compPlayerCam->SetRelativeLocation(FVector(-15, 0, 12));
	compPlayerCam->SetRelativeRotation(FRotator(-35, 0, 0)); //★★★추후 필요시 변경
	compPlayerCam->bUsePawnControlRotation = false; //입력에 따른 회전 설정

	bUseControllerRotationYaw = true; //입력에 따른 회전 설정


	
	//[총 컴포넌트 추가] --->스켈레탈 무기
	compMeshGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMeshComp"));
	compMeshGun->SetupAttachment(GetMesh(), TEXT("Gun")); //부모 컴포넌트를 Mesh 컴포넌트로 설정. 총을 캐릭터 손 소켓에 붙임...?
	
	//[총 스켈레탈메시 추가. 옷 추가]
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempGunMesh(TEXT("StaticMesh'/Game/Geometry/Gun.Gun'"));
	if (tempGunMesh.Succeeded())
	{
		compMeshGun->SetStaticMesh(tempGunMesh.Object);
		compMeshGun->SetRelativeLocation(FVector(5.759851f, -0.573986f, 26.993545f)); //★★★총 위치. 임시로 세팅. 추후 변경 필요
		compMeshGun->SetRelativeRotation(FRotator(0, -159, - 90)); //★★★총 회전. 임시로 세팅. 추후 변경 필요
		compMeshGun->SetRelativeScale3D(FVector(0.5f)); //★★★총 크기. 임시로 세팅. 추후 변경 필요
	}
	
	//[롤리팝 컴포넌트 추가] --->스태틱 무기
	//compMeshLollipop->CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LollipopMeshComp"));
	//compMeshLollipop->SetupAttachment(GetMesh(), TEXT("Lollipop")); //부모 컴포넌트를 Mesh 컴포넌트로 설정. 롤리팝을 캐릭터 손 소켓에 붙임...?

	//[롤리팝 스테틱메시 추가. 옷 추가]
	//ConstructorHelpers::FObjectFinder<UStaticMesh> tempLollipopMesh(TEXT("StaticMesh'/Game/Geometry/Lollipop.Lollipop'"));
	//if (tempLollipopMesh.Succeeded())
	//{
	//	compMeshLollipop->SetStaticMesh(tempLollipopMesh.Object);
	//}



	//[플레이어 펀치 충돌 컴포넌트 추가]
	compCollisionPunchR = CreateDefaultSubobject<UBoxComponent>(TEXT("PunchMeshCompR"));
	compCollisionPunchL = CreateDefaultSubobject<UBoxComponent>(TEXT("PunchMeshCompL"));
	compCollisionPunchR->SetRelativeScale3D(FVector(0.5f));
	compCollisionPunchL->SetRelativeScale3D(FVector(0.5f));
	compCollisionPunchR->SetupAttachment(GetMesh(), TEXT("Prop_RSocket")); //오른손. 충돌 컴포넌트를 캐릭터 소켓에 붙임
	compCollisionPunchL->SetupAttachment(GetMesh(), TEXT("Prop_LSocket")); //왼손. 충돌 컴포넌트를 캐릭터 소켓에 붙임

// 	//[플레이어 킥 충돌 컴포넌트 추가]
// 	compCollisionKick = CreateDefaultSubobject<UBoxComponent>(TEXT("KickMeshComp"));
// 	//compCollisionKick->SetRelativeLocation(FVector());
// 	//compCollisionKick->SetRelativeScale3D(FVector(0.5f));
// 	compCollisionKick->SetupAttachment(GetMesh(), TEXT("ball_lSocket"));
// 
// 	//[플레이어 헤딩 충돌 컴포넌트 추가]
// 	compCollisionHeadbutt = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadbuttMechComp"));
// 	//compCollisionHeadbutt->SetRelativeScale3D(FVector(0.5f));
// 	compCollisionHeadbutt->SetupAttachment(GetMesh(), TEXT("headSocket"));


	//불렛팩토리 안에 불렛 디폴트로 추가 ★★★왜 이 작업이 필요한지 궁금
	ConstructorHelpers::FClassFinder<ARIM_Bullet> tempBul(TEXT("Blueprint'/Game/BluePrint/BP_RIM_Bullet.BP_RIM_Bullet_C'")); //오류. C++ 파일 찾을 수 없다고하여 블루프린트 파일로 진행
	if(tempBul.Succeeded())
	{
		bulletFactory = tempBul.Class;
	}

	//애니메이션...?
// 	ConstructorHelpers::FClassFinder<URIM_PlayerAnim> tempAnim(TEXT("AnimBlueprint'/Game/BluePrint/ABP_RIM_PlayerAnim.ABP_RIM_PlayerAnim_C'"));
// 	if (tempAnim.Succeeded())
// 	{
// 		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
// 	}

	//무기 버리기 관련
	ConstructorHelpers::FClassFinder<ARIM_Gun> TempGun(TEXT("Blueprint'/Game/BluePrint/BP_RIM_Gun.BP_RIM_Gun_c'"));
	if (TempGun.Succeeded())
	{
		Gun = TempGun.Class;
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

	//[롤리팝이 디폴트로 안 보이게 설정]
	//compMeshLollipop->SetVisibility(false);


	//[[1]플레이어 주먹 콜리전과 에너미가 충돌 시 실행 될 함수 호출]
	//펀치 콜리전 컴포넌트 변수의 멤버 함수 OnComponentBeginOverlap과 OnComponentEndOverlap 델리케이트 호출 + 펀치 콜리전 컴포턴트에 밑에 구현한 함수 연결
	compCollisionPunchR->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionPunchRBeginOverlap);
	compCollisionPunchL->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionPunchLBeginOverlap);
	compCollisionPunchR->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionPunchREndOverlap);
	compCollisionPunchL->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionPunchLEndOverlap);

	//[1]플레이어 킥 콜리전과 에너미가 출돌 시 실행 될 함수 호출
// 	compCollisionKick->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionKickBeginOverlap);
// 	compCollisionKick->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionKickEndOverlap);

	//[1]플레이어 박치기 콜리전과 에너미가 충돌 시 실행 될 함수 호출
// 	compCollisionHeadbutt->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionHeadbuttBeginOverlap);
// 	compCollisionHeadbutt->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionHeadbuttEndOverlap);



}

// Called every frame
void ARIM_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//[Move(플레이어 이동) 함수 호출]
	Move();


}

//[Move(플레이어 이동) 함수 구현]
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

	//[무기 버리기 이벤트 처리 함수 바인딩/호출]
	PlayerInputComponent->BindAction(TEXT("DropWeapon"), IE_Pressed, this, &ARIM_Player::InputDropWeapon);
	
	//[드롭킥/던지기 이벤트 처리 함수 바인딩/호출]
	//PlayerInputComponent->BindAction(TEXT("KickToss"), IE_Pressed, this, &ARIM_Player::InputKickToss);

	
	//[박치기 이벤트 처리 함수 바인딩/호출]
	//PlayerInputComponent->BindAction(TEXT("Headbutt"), IE_Pressed, this, &ARIM_Player::InputHeadbutt);



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
	else //만약 플레이어가 총을 들고 있지 않을 때
	{
		if (isInputPunchGrab == true) //isInputPunchGrab 키를 누르면
		{
			//펀치 애니메이션을 플레이한다

			ASH_Enemy* Enemy = Cast<ASH_Enemy>(currEnemy); //에너미 형변환
			Enemy->fsm->OnDamageProcess(); //에너미(액터 상속 받음)에서 fsm(액터 컴포넌트 상속 받음)에서 데미지프로세스 호출
		}
	}
}



//[무기 버리기 이벤트 처리 함수 구현]
void ARIM_Player::InputDropWeapon()
{
	//스켈무기, 스태틱무기 2개 필요
	
	//플레이어가 가지고 있는 무기 컴포넌트가 안보이게한다
	if (compMeshGun->IsVisible())
	{
		GetWorld()->SpawnActor<ARIM_Gun>(Gun, GetActorLocation(), GetActorRotation());	
	}
	compMeshGun->SetVisibility(false);
	//무기가 플레이어 위치(바닥)에 스폰된다
}



//[플레이어가 들고 있는 총이 안 보였다 보이는 함수 구현]
void ARIM_Player::VisibleGun()
{
	//GetMesh()->SetSkeletalMesh(상호작용한)

	compMeshGun->SetVisibility(true);
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

}



// [드롭킥/던지기 이벤트 처리 함수 구현]
// void ARIM_Player::InputKickToss()
// {
// 	//드롭킥 에니메이션을 플레이한다
//  
// 	ASH_Enemy* Enemy = Cast<ASH_Enemy>(currEnemy); //에너미 형변환
// 	Enemy->fsm->OnDamageProcess(); //에너미(액터 상속 받음)에서 fsm(액터 컴포넌트 상속 받음)에서 데미지프로세스 호출
// }



// [박치기 이벤트 처리 함수 구현]
// void ARIM_Player::InputHeadbutt()
// {
// 	//박치기 애니메이션을 플레이한다.
// 
// 	ASH_Enemy* Enemy = Cast<ASH_Enemy>(currEnemy); //에너미 형변환
// 	Enemy->fsm->OnDamageProcess(); //에너미(액터 상속 받음)에서 fsm(액터 컴포넌트 상속 받음)에서 데미지프로세스 호출
// }



//[???]
void ARIM_Player::EnableInput(APlayerController* PlayerController)
{
	Super::EnableInput(PlayerController);

	//[공격/잡기 이벤트 처리 함수 바인딩/호출] = 총알 발사. Gun에 바인딩 넘겼다가 플레이어가 가져옴
	PlayerController->InputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, &ARIM_Player::InputPunchGrab);
}



//[플레이어 주먹 콜리전과 에너미가 충돌 시 실행할 내용/함수 구현]
void ARIM_Player::collisionPunchRBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//만약에 부딛힌 OtherActor의 이름이 enemy를 포함하고있으면
	if (OtherActor->GetName().Contains(TEXT("Enemy")))
	{

		isInputPunchGrab = true;
		currEnemy = OtherActor;
	}
}

//[플레이어 주먹 콜리전과 에너미가 충돌 시 실행할 내용/함수 구현]
void ARIM_Player::collisionPunchLBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//만약에 부딛힌 OtherActor의 이름이 enemy를 포함하고있으면
	if (OtherActor->GetName().Contains(TEXT("Enemy")))
	{
		isInputPunchGrab = true;
		currEnemy = OtherActor;
	}
}

//[플레이어 주먹 콜리전과 에너미가 충돌 시 실행할 내용/함수 구현]
void ARIM_Player::collisionPunchREndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputPunchGrab = false;
}

//[플레이어 주먹 콜리전과 에너미가 충돌 시 실행할 내용/함수 구현]
void ARIM_Player::collisionPunchLEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputPunchGrab = false;
}



//[플레이어 킥 콜리전과 에너미가 충돌 시 실행할 내용/함수 구현]
// void ARIM_Player::collisionKickBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	//만약에 부딛힌 OtherActor의 이름이 enemy를 포함하고있으면
// 	if (OtherActor->GetName().Contains(TEXT("Enemy")))
// 	{
// 		isInputKickToss = true;
// 		currEnemy = OtherActor;
// 
// 	}
// }

//[플레이어 킥 콜리전과 에너미가 충돌 시 실행할 내용/함수 구현]
// void ARIM_Player::collisionKickEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// 	isInputKickToss = false;
// }



//[플레이어 박치기 콜리전과 에너미가 충돌 시 실행할 내용/함수 구현]
// void ARIM_Player::collisionHeadbuttBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	//만약에 부딛힌 OtherActor의 이름이 enemy를 포함하고있으면
// 	if (OtherActor->GetName().Contains(TEXT("Enemy")))
// 	{
// 		isInputHeadbutt = true;
// 		currEnemy = OtherActor;
//  
// 	}
// }

//[플레이어 박치기 콜리전과 에너미가 충돌 시 실행할 내용/함수 구현]
// void ARIM_Player::collisionHeadbuttEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// 	isInputHeadbutt = false;
// }



//[플레이어가 공격 당하면 에너미에서 호출하는 함수] 플레이어의 데미지니까 플레이어에서 구현
void ARIM_Player::OnDamageProcess()
{
	UE_LOG(LogTemp, Warning, TEXT("Damaged!!!")); //임시로 텍스트 출력
	
	if (HP > 0)
	{
		HP--; //플레이어 HP 1씩 감소
	}
	else //플레이어가 에너미한테 5번 공격 받으면(HP가 5이고 공격 받을때마다 HP가 1씩 줄어드니까 5번 공격 받는 것은 HP가 0이 되는 것과 동일하다)
	{
		UE_LOG(LogTemp, Warning, TEXT("I'll be back!")); //플레이어가 기절한다(애니메이션). 임시로 텍스트 출력
		
		//기절하면 대미지 않들어감

		currentTime += GetWorld()->GetDeltaSeconds(); //현재시간 + 델타타임 = 현재 누적시간
		if (currentTime > 5) //플레이어가 기절한 시간(5초)가 지나면
		{	
			UE_LOG(LogTemp, Warning, TEXT("Hello!")); //플레이어가 일어난다(애니메이션). 임시로 텍스트 출력
			currentTime = 0; //현재 시간을 0초로 초기화 한다
			HP = 5; //플레이어의 HP를 5초로 초기화 한다
		}
	}
}