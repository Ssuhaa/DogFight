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
#include <Components/StaticMeshComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SphereComponent.h>
#include "GunWeapon.h"
#include "LollipopWeapon.h"
#include "CharacterSpawn.h"
#include "ItemSpawn.h"
#include "../DogFightGameModeBase.h"

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

	//[플레이어 본체 충돌 컴포넌트 Collision 세팅]
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player")); //프리셋 Player 로 수정 
	
	

	//[스프링암 컴포넌트 추가. USpringArmComponent]
	compSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	compSpringArm->SetupAttachment(RootComponent); //루트컴포넌트에 자식으로 붙임
	compSpringArm->SetRelativeLocation(FVector(0, 0, 0)); //▶추후 필요시 변경
	compSpringArm->SetRelativeRotation(FRotator(0, 0, 0)); //▶추후 필요시 변경
	compSpringArm->TargetArmLength = 900; //▶추후 필요시 변경
	compSpringArm->SocketOffset = FVector(0, 0, 450); //▶추후 필요시 변경
	compSpringArm->bUsePawnControlRotation = true; //입력에 따른 회전 설정

	//[카메라 컴포넌트 추가. UCameraComponent]
	compPlayerCam = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamComp"));
	compPlayerCam->SetupAttachment(compSpringArm);
	compPlayerCam->SetRelativeLocation(FVector(0, 0, 1));
	compPlayerCam->SetRelativeRotation(FRotator(-25, 0, 0)); //▶추후 필요시 변경
	compPlayerCam->bUsePawnControlRotation = false; //입력에 따른 회전 설정

	bUseControllerRotationYaw = false; //입력에 따른 회전 설정. 이렇게 설정해야지 캐릭터가 360 돌아간다...
	GetCharacterMovement()->bOrientRotationToMovement = true; //움직일 때 플레이어 뒤통수 안 보이고 다 보이게 한다
	
	//[총 스태틱메시 컴포넌트 추가]
	compMeshGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMeshComp"));
	compMeshGun->SetupAttachment(GetMesh(), TEXT("Gun")); //부모 컴포넌트를 Mesh 컴포넌트로 설정. 총을 캐릭터 손 소켓에 붙임...?
		
	//[총 스태틱메시 추가. 옷 추가] 
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempGunMesh(TEXT("StaticMesh'/Game/Geometry/Gun.Gun'")); 
	if (tempGunMesh.Succeeded())
	{
		compMeshGun->SetStaticMesh(tempGunMesh.Object);
		//compMeshGun->SetRelativeLocation(FVector(-8.280492f, 21.242059f, 5.475584f)); //▶총 위치. 임시로 세팅. 추후 변경 필요
		//compMeshGun->SetRelativeRotation(FRotator(60.000000f, -339.000000f, -270.000000f)); //▶총 회전. 임시로 세팅. 추후 변경 필요
	}
	
	

	//[롤리팝 스태틱메시 컴포넌트 추가]
	compMeshLollipop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LollipopMeshComp"));
	compMeshLollipop->SetupAttachment(GetMesh(), TEXT("Lollipop")); //부모 컴포넌트를 Mesh 컴포넌트로 설정. 롤리팝을 캐릭터 손 소켓에 붙임...?

	//[롤리팝 스테틱메시 추가. 옷 추가]
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempLollipopMesh(TEXT("StaticMesh'/Game/Geometry/Lollipop.Lollipop'"));
	if (tempLollipopMesh.Succeeded())
	{
		compMeshLollipop->SetStaticMesh(tempLollipopMesh.Object);
		compMeshLollipop->SetRelativeLocation(FVector(0.686449f, -9.618860f, 2.343055f));
		compMeshLollipop->SetRelativeRotation(FRotator(0, 0, -20));
	}

	//[롤리팝 충돌체 컴포넌트 추가]
	compCollisionLollipop = CreateDefaultSubobject<USphereComponent>(TEXT("LollipopCollisionComp"));
	compCollisionLollipop->SetupAttachment(compMeshLollipop); //★★★엔진에 적용 안됨
	compCollisionLollipop->SetSphereRadius(35); //▶추후 필요시 변경
	compCollisionLollipop->SetRelativeLocation(FVector(0.5f, 0, 40)); //▶추후 필요시 변경 



	//[플레이어 펀치 충돌 컴포넌트 추가]
	compCollisionPunchR = CreateDefaultSubobject<UBoxComponent>(TEXT("PunchCollisionCompR"));
	compCollisionPunchL = CreateDefaultSubobject<UBoxComponent>(TEXT("PunchCollisionCompL"));
	compCollisionPunchR->SetRelativeLocation(FVector(0, 6, -9));
	compCollisionPunchR->SetRelativeScale3D(FVector(0.6f, 0.5f, 0.5f));
	compCollisionPunchL->SetRelativeScale3D(FVector(0.6f, 0.5f, 0.5f));
	compCollisionPunchR->SetupAttachment(GetMesh(), TEXT("Prop_RSocket")); //메시의 자식으로 세팅. 오른손. 충돌 컴포넌트를 캐릭터 소켓에 붙임
	compCollisionPunchL->SetupAttachment(GetMesh(), TEXT("Prop_LSocket")); //메시의 자식으로 세팅. 왼손. 충돌 컴포넌트를 캐릭터 소켓에 붙임

 	//[플레이어 킥 충돌 컴포넌트 추가]
 	compCollisionKick = CreateDefaultSubobject<UBoxComponent>(TEXT("KickCollisionComp"));
	compCollisionKick->SetRelativeLocation(FVector(26, 10, 6));
	compCollisionKick->SetRelativeScale3D(FVector(0.8f, 0.8f, 1));
 	compCollisionKick->SetupAttachment(GetMesh(), TEXT("ball_lSocket")); //메시의 자식으로 세팅. 왼발. 충돌 컴포넌트를 캐릭터 소켓에 붙임
 
	//[플레이어 헤딩 충돌 컴포넌트 추가]
 	compCollisionHeadbutt = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadbuttCollisionComp"));
	compCollisionHeadbutt->SetRelativeLocation(FVector(0, 48, 193));
 	compCollisionHeadbutt->SetRelativeScale3D(FVector(1.5f, 1.7f, 1.2f));
 	compCollisionHeadbutt->SetupAttachment(GetMesh(), TEXT("headSocket")); //메시의 자식으로 세팅. 머리. 충돌 컴포넌트를 캐릭터 소켓에 붙임



	//[불렛팩토리 안에 불렛 디폴트로 추가] ★★★왜 이 작업이 필요한지 궁금
	ConstructorHelpers::FClassFinder<ARIM_Bullet> tempBul(TEXT("Blueprint'/Game/BluePrint/BP_RIM_Bullet.BP_RIM_Bullet_C'")); //★★★오류. 계속 오류나면 주석처리하고 블루프린트에서 적용 할 것
	if(tempBul.Succeeded())
	{
		bulletFactory = tempBul.Class; //플레이어에 추가. Bullet Factory 에 드롭다운으로 BP_RIM_Bullet 추가
	}

	//[총 무기 버리기 추가]
	ConstructorHelpers::FClassFinder<AGunWeapon> tempGun(TEXT("Class'/Script/DogFight.GunWeapon'"));
	if (tempGun.Succeeded())
	{
		weaponGun = tempGun.Class; //플레이어에 추가. WeaponGun 에 드롭다운으로 GunWeapon 추가
	}

	//[롤리팝 무기 버리기 추가]
	ConstructorHelpers::FClassFinder<ALollipopWeapon> tempLollipop(TEXT("Class'/Script/DogFight.LollipopWeapon'"));
	if (tempLollipop.Succeeded())
	{
		weaponLollipop = tempLollipop.Class; //플레이어에 추가. WeaponLollipop 에 드롭다운으로 LollipopWeapon 추가
	}

	//[애니메이션 디폴트로 적용]
	ConstructorHelpers::FClassFinder<URIM_PlayerAnim> tempAnim(TEXT("AnimBlueprint'/Game/BluePrint/ABP_Player.ABP_Player_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class); //플레이어->메시->애니메이션->애님클래스 에 추가. Anim Class 에 드롭다운으로 ABP_Player 추가
	}




}

// Called when the game starts or when spawned
void ARIM_Player::BeginPlay()
{
	Super::BeginPlay();

	//[초기 속도를 걷기로 설정]
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	//[총이 디폴트로 안보이게 설정]
	compMeshGun->SetVisibility(false); //시작할 때 플레이어가 총을 들고 있지 않는다. 총이 안 보인다. 

	//[롤리팝이 디폴트로 안 보이게 설정]
	compMeshLollipop->SetVisibility(false); //시작할 때 플레이어가 롤리팝을 들고 있지 않는다. 롤리팝이 안 보인다.


	//[[1]플레이어 주먹 콜리전과 에너미가 충돌 시 실행 될 함수 호출] ★★★완벽하게 이해하지 못 함. 왜 BeginPlay에서 하는지???
	//펀치 콜리전 컴포넌트 변수의 멤버 함수 OnComponentBeginOverlap과 OnComponentEndOverlap 델리케이트 호출 + 펀치 콜리전 컴포턴트에 밑에 구현한 함수 연결
	compCollisionPunchR->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionPunchRBeginOverlap);
	compCollisionPunchL->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionPunchLBeginOverlap);
	compCollisionPunchR->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionPunchREndOverlap);
	compCollisionPunchL->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionPunchLEndOverlap);

	//[1]플레이어 킥 콜리전과 에너미가 충돌 시 실행 될 함수 호출
	compCollisionKick->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionKickBeginOverlap);
	compCollisionKick->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionKickEndOverlap);

	//[1]플레이어 박치기 콜리전과 에너미가 충돌 시 실행 될 함수 호출
	compCollisionHeadbutt->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisionHeadbuttBeginOverlap);
	compCollisionHeadbutt->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisionHeadbuttEndOverlap);

	//[1]플레이어가 든 롤리팝 콜리전과 에너미가 충돌 시 실행 될 함수 호출
	compCollisionLollipop->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Player::collisonLollipopBeginOverlap);
	compCollisionLollipop->OnComponentEndOverlap.AddDynamic(this, &ARIM_Player::collisonLollipopEndOverlap);

	//애니메이션 관련 ★★★???
	animPlayer = Cast<URIM_PlayerAnim>(GetMesh()->GetAnimInstance());

}

// Called every frame
void ARIM_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//[Move(플레이어 이동) 함수 호출]
	Move();

	//[플레이어 기절 후 일어나는 코드]
	if (isplayerDie == false && isplayerDown == true) //플레이어가 살아있고 + 플레이어가 누워있으면
	{
		currentTime += GetWorld()->DeltaTimeSeconds; //현재시간 + 델타타임 = 현재 누적시간

		if (currentTime > 5) //플레이어가 기절한 시간(5초)가 지나면
		{
			//플레이어 애니메이션 몽타주 중 '업' 애니메이션 랜덤 재생
			rand = FMath::RandRange(0, 1);
			animPlayer->PlayPlayerTwoAnim(TEXT("WakeUp"), rand);

			UE_LOG(LogTemp, Error, TEXT("Player Wake up!")); // 확인용 텍스트 출력
						
			currentTime = 0; //현재 시간을 0초로 초기화 한다
			HP = 5; //플레이어의 HP를 5초로 초기화 한다

			isplayerDown = false; //플레이어를 일어나게한다. 키 누르면 움직이게 하기 위해 추가
		}
	}

	//[실패 위젯 띄운다]
	if (isplayerDie == true) //플레이어가 죽은 상태일 때
	{
		currentTime += DeltaTime;

		if (currentTime > 2)
		{
			UGameplayStatics::OpenLevel(GetWorld(), "Result"); //결과 레벨(화면)을 연다.
		}
	}
}

//[Move(플레이어 이동) 함수 구현]
void ARIM_Player::Move()
{
	if (isplayerDown == false) //플레이어가 서있을 때만 이동 가능하게 한다
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
		AddMovementInput(direction);
		direction = FVector::ZeroVector;	
	}
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
		PlayerInputComponent->BindAction(TEXT("KickToss"), IE_Pressed, this, &ARIM_Player::InputKickToss);
	
		//[박치기 이벤트 처리 함수 바인딩/호출]
		PlayerInputComponent->BindAction(TEXT("Headbutt"), IE_Pressed, this, &ARIM_Player::InputHeadbutt);

		//[무기 버리기 이벤트 처리 함수 바인딩/호출]
		PlayerInputComponent->BindAction(TEXT("DropWeapon"), IE_Pressed, this, &ARIM_Player::InputDropWeapon);
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

// 	ACharacterSpawn* spawn = Cast<ACharacterSpawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterSpawn::StaticClass()));
// 	ASH_Enemy* enemy = spawn->spawnedEnemy[spawn->spawnedEnemy.Num() - 1];
// 	enemy->fsm->stateChangeMontage(EEnemyState::Die, TEXT("Die"));
// 	spawn->spawnedEnemy.Remove(enemy);
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


//############### 총알 발사, 롤리팝 + 펀치 ###############
//[공격/잡기 이벤트 처리 함수 구현] = 총알 발사 + 롤리팝 + 펀치
void ARIM_Player::InputPunchGrab()
{	
	if (isplayerDown == false) //플레이어가 서있을 때
	{
		if (compMeshGun->IsVisible() == true) //만약 플레이어가 총을 들고 있으면(플레이어가 든 총이 보이면) -> 총에서 총알을 발사한다
		{
			UE_LOG(LogTemp, Error, TEXT("Player Fire!!!")); //확인용 텍스트 출력

			//플레이어 애니메이션 몽타주 중 '파이어' 애니메이션 재생
			animPlayer->PlayPlayerAnim(TEXT("Fire"), 0);

			//총알 발사
			//FTransform trFire = compMeshGun->GetSocketTransform(TEXT("FirPos")); //총 스켈레톤에서 소켓 추가 ---> 처음에 총 스켈레톤메시일 때 사용 했던 코드
			//GetWorld()->SpawnActor<ARIM_Bullet>(bulletFactory, trFire); ////총의 총구에서 총알을 발사한다. 월드에서 ARIM_Bullet(총알) 가져온다 ---> 처음에 총 스켈레톤메시일 때 사용 했던 코드

			GetWorld()->SpawnActor<ARIM_Bullet>(bulletFactory, GetActorLocation() + GetActorForwardVector() * 200, GetActorRotation());
		}
		else if (compMeshLollipop->IsVisible() == true) //플레이어가 롤리팝을 들고 있지 않고, 롤리팝을 들고 있으면(플레이어가 든 롤리팝이 보이면) -> 롤리팝으로 공격한다
		{
			UE_LOG(LogTemp, Error, TEXT("Player Lollipop!")); //확인용 텍스트 출력

			//플레이어 애니메이션 몽타주 중 '롤리팝' 애니메이션 재생
			animPlayer->PlayPlayerTwoAnim(TEXT("Lollipop"), 0);

			if (isInputPunchGrab == true) //플레이어가 롤리팝을 든 상태로 isInputPunchGrab 키를 누르면
			{
				UE_LOG(LogTemp, Error, TEXT("Player Lollipop Attack!")); //확인용 텍스트 출력

				Enemy = Cast<ASH_Enemy>(currEnemy); //에너미 형변환! 했을 때 널인지 아닌지 꼭 체크해야 한다
				if (Enemy != nullptr) //에너미가 아니다 = 에너미이다. 플레이어 롤리팝에 맞은 것이 에너미가 아닐 수도 있기 때문에 에너미인지 확인 필요
				{
					//에너미가 대기, 이동, 공격, 무기 들기 상태 일 때
					if (Enemy->fsm->mState == EEnemyState::Idle || Enemy->fsm->mState == EEnemyState::Move || Enemy->fsm->mState == EEnemyState::Attack || Enemy->fsm->mState == EEnemyState::Pickup)
					{
						Enemy->fsm->OnDamageProcess(); //에너미 맞으면 데미지가 들어간다. 에너미(액터 상속 받음)에서 fsm(액터 컴포넌트 상속 받음)에서 데미지프로세스 호출
					}
				}
			}
		}
		else //플레이어가 총을 들고 있지 않고, 롤리팝을 들고 있지 않고 InputPunchGrab 버튼을 누르면 -> 펀치한다 
		{
			UE_LOG(LogTemp, Error, TEXT("Player Punch!")); //확인용 텍스트 출력

			//플레이어 애니메이션 몽타주 중 '펀치' 애니메이션 재생
			animPlayer->PlayPlayerAnim(TEXT("Punch"), 0);

			if (isInputPunchGrab == true) //isInputPunchGrab 키를 누르면
			{
				UE_LOG(LogTemp, Error, TEXT("Player Punch Attack!")); //확인용 텍스트 출력

				Enemy = Cast<ASH_Enemy>(currEnemy); //에너미 형변환! 했을 때 널인지 아닌지 꼭 체크해야 한다
				if (Enemy != nullptr) //에너미가 아니다 = 에너미이다. 플레이어 펀치에 맞은 것이 에너미가 아닐 수도 있기 때문에 에너미인지 확인 필요
				{
					//에너미가 대기, 이동, 공격, 무기 들기 상태 일 때
					if (Enemy->fsm->mState == EEnemyState::Idle || Enemy->fsm->mState == EEnemyState::Move || Enemy->fsm->mState == EEnemyState::Attack || Enemy->fsm->mState == EEnemyState::Pickup)
					{
						Enemy->fsm->OnDamageProcess(); //에너미 맞으면 데미지가 들어간다. 에너미(액터 상속 받음)에서 fsm(액터 컴포넌트 상속 받음)에서 데미지프로세스 호출
					}
				}
			}
		}
	}
}

//[[3]BeginOverlap 플레이어 오른주먹 콜리전과 에너미가 충돌 시 실행할 내용/함수 구현]
void ARIM_Player::collisionPunchRBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains(TEXT("Enemy"))) //만약에 부딛힌 OtherActor 중에 이름이 Enemy 를 포함하고있으면
	{
		isInputPunchGrab = true;
		currEnemy = OtherActor; //부딛힌 OtherActor(이름 중에 Enemy 를 포함하고 있는)가 SH_Enemy 클래스의 Enemy 이면 currEnemy 라고 한다
	}
}

//[[3]BeginOverlap 플레이어 왼주먹 콜리전과 에너미가 충돌 시 실행할 내용/함수 구현]
void ARIM_Player::collisionPunchLBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains(TEXT("Enemy"))) //만약에 부딛힌 OtherActor 중에 이름이 Enemy 를 포함하고있으면
	{
		isInputPunchGrab = true;
		currEnemy = OtherActor; //부딛힌 OtherActor(이름 중에 Enemy 를 포함하고 있는)가 SH_Enemy 클래스의 Enemy 이면 currEnemy 라고 한다
	}
}

//[[3]EndOverlap 플레이어 오른주먹 콜리전과 에너미가 충돌 시 실행할 내용/함수 구현]
void ARIM_Player::collisionPunchREndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputPunchGrab = false;
}

//[[3]EndOverlap 플레이어 왼주먹 콜리전과 에너미가 충돌 시 실행할 내용/함수 구현]
void ARIM_Player::collisionPunchLEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputPunchGrab = false;
}


//[[3]BeginOverlap 플레이어가 든 롤리팝 콜리전과 에너미가 충돌 시 실행할 내용/함수 구현]
void ARIM_Player::collisonLollipopBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains(TEXT("Enemy"))) //만약에 부딛힌 OtherActor 중에 이름이 Enemy 를 포함하고있으면
	{
		isInputPunchGrab = true;
		currEnemy = OtherActor; //부딛힌 OtherActor(이름 중에 Enemy 를 포함하고 있는)가 SH_Enemy 클래스의 Enemy 이면 currEnemy 라고 한다
	}
}

//[[3]EndOverlap 플레이어가 든 롤리팝 콜리전과 에너미가 충돌 시 실행할 내용/함수 구현]
void ARIM_Player::collisonLollipopEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputPunchGrab = false;
}



//############### 킥 ###############
// [드롭킥/던지기 이벤트 처리 함수 구현]
void ARIM_Player::InputKickToss()
{
	if (isplayerDown == false) //플레이어가 서있을 때
	{
		//플레이어 애니메이션 몽타주 중 '킥' 애니메이션 재생
		animPlayer->PlayPlayerTwoAnim(TEXT("Kick"), 0);

		UE_LOG(LogTemp, Error, TEXT("Player Kick!")); //확인용 텍스트 출력

		if(isInputKickToss == true)
		{
			UE_LOG(LogTemp, Error, TEXT("Player Kick Attack!")); //확인용 텍스트 출력

			Enemy = Cast<ASH_Enemy>(currEnemy); //에너미 형변환! 했을 때 널인지 아닌지 꼭 체크해야 한다
			if (Enemy != nullptr) //에너미가 아니다 = 에너미이다. 플레이어 펀치에 맞은 것이 에너미가 아닐 수도 있기 때문에 에너미인지 확인 필요
			{
				//에너미가 대기, 이동, 공격, 무기 들기 상태 일 때
				if (Enemy->fsm->mState == EEnemyState::Idle || Enemy->fsm->mState == EEnemyState::Move || Enemy->fsm->mState == EEnemyState::Attack || Enemy->fsm->mState == EEnemyState::Pickup)
				{
					Enemy->fsm->OnDamageProcess(); //에너미 맞으면 데미지가 들어간다. 에너미(액터 상속 받음)에서 fsm(액터 컴포넌트 상속 받음)에서 데미지프로세스 호출
				}
			}
		}
	}
}

//[[3]BeginOverlap 플레이어 킥 콜리전과 에너미가 충돌 시 실행할 내용/함수 구현]
void ARIM_Player::collisionKickBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains(TEXT("Enemy"))) //만약에 부딛힌 OtherActor 중에 이름이 Enemy 를 포함하고있으면
	{
		isInputKickToss = true;
		currEnemy = OtherActor; //부딛힌 OtherActor(이름 중에 Enemy 를 포함하고 있는)가 SH_Enemy 클래스의 Enemy 이면 currEnemy 라고 한다
	}
}

//[[3]EndOverlap 플레이어 킥 콜리전과 에너미가 충돌 시 실행할 내용/함수 구현]
void ARIM_Player::collisionKickEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputKickToss = false; 
}



//############### 박치기 ###############
// [박치기 이벤트 처리 함수 구현]
void ARIM_Player::InputHeadbutt() //헤딩 키 누르면 헤딩 한다(애니메이션) > 
{
	if (isplayerDown == false) //플레이어가 서있을 때
	{
		//플레이어 애니메이션 몽타주 중 '헤딩' 애니메이션 재생
		animPlayer->PlayPlayerAnim(TEXT("Headbutt"), 0);

		UE_LOG(LogTemp, Error, TEXT("Player Headbutt!")); //확인용 텍스트 출력

		if (isInputHeadbutt == true)
		{
			UE_LOG(LogTemp, Error, TEXT("Player Headbutt Attack!")); //확인용 텍스트 출력

			Enemy = Cast<ASH_Enemy>(currEnemy); //에너미 형변환! 했을 때 널인지 아닌지 꼭 체크해야 한다
			if (Enemy != nullptr) //에너미가 아니다 = 에너미이다. 플레이어 펀치에 맞은 것이 에너미가 아닐 수도 있기 때문에 에너미인지 확인 필요
			{
				//에너미가 대기, 이동, 공격, 무기 들기 상태 일 때
				if (Enemy->fsm->mState == EEnemyState::Idle || Enemy->fsm->mState == EEnemyState::Move || Enemy->fsm->mState == EEnemyState::Attack || Enemy->fsm->mState == EEnemyState::Pickup)
				{
					Enemy->fsm->OnDamageProcess(); //에너미 맞으면 데미지가 들어간다. 에너미(액터 상속 받음)에서 fsm(액터 컴포넌트 상속 받음)에서 데미지프로세스 호출
				}
			}
		}
	}
}

//[[3]BeginOverlap 플레이어 박치기 콜리전과 에너미가 충돌 시 실행할 내용/함수 구현]
void ARIM_Player::collisionHeadbuttBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains(TEXT("Enemy"))) //만약에 부딛힌 OtherActor 중에 이름이 Enemy 를 포함하고있으면
	{
		isInputHeadbutt = true;
		currEnemy = OtherActor; //부딛힌 OtherActor(이름 중에 Enemy 를 포함하고 있는)가 SH_Enemy 클래스의 Enemy 이면 currEnemy 라고 한다
	}
}

//[[3]EndOverlap 플레이어 박치기 콜리전과 에너미가 충돌 시 실행할 내용/함수 구현]
void ARIM_Player::collisionHeadbuttEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isInputHeadbutt = false;
}



//[플레이어에 총이 보이게 하는 함수 구현]
void ARIM_Player::VisibleGun()
{
	compMeshGun->SetVisibility(true); //플레이어가 든 총이 보이게 한다
	//바닥의 무기 콜리전과 닿으면 바닥의 무기가 파괴되지 않는다? 바닥의 무기 콜리전을 무시한다? 바닥의 무기 콜리전을 감지하지 못한다?
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0)); //★★★ 버튼(키) 관련...? 모르겠음. 궁금함

	isPlayerVisibleGun = true;

}

//[플레이어에 롤리팝이 보이게 하는 함수 구현]
void ARIM_Player::VisibleLollipop()
{
	compMeshLollipop->SetVisibility(true); //플레이어가 든 롤리팝이 보이게 한다
	//바닥의 무기 콜리전과 닿으면 바닥의 무기가 파괴되지 않는다? 바닥의 무기 콜리전을 무시한다? 바닥의 무기 콜리전을 감지하지 못한다?
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0)); //★★★ 버튼(키) 관련...? 모르겠음. 궁금함

	isPlayerVisibleLollipop = true;

}

//[총, 롤리팝 관련 ??? 함수 구현] //★★★ 잘 모르겠음. 왜 만들어야 할까?
void ARIM_Player::EnableInput(APlayerController* PlayerController)
{
	Super::EnableInput(PlayerController);

	//[공격/잡기 이벤트 처리 함수 바인딩/호출] = 총알 발사. Weapon에 바인딩 넘겼다가 플레이어가 가져옴
	PlayerController->InputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, &ARIM_Player::InputPunchGrab);
}



//############### 무기 바닥에 버리기 ###############
// [무기 버리기 이벤트 처리 함수 구현]
void ARIM_Player::InputDropWeapon()
{	
	AItemSpawn* ItemSpawn = Cast<AItemSpawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AItemSpawn::StaticClass()));
		if (isPlayerVisibleGun == true) //만약 플레이어의 총이 보이면(플레이어가 총을 들고 있을 때)
		{
			compMeshGun->SetVisibility(false); //플레이어의 총 컴포넌트가 안보이게한다
			//GetWorld()->SpawnActor<AGunWeapon>(weaponGun, GetActorLocation() + FVector(-200,0,0), GetActorRotation()); //GunWeapon이 플레이어 위치의 바닥에 스폰된다
			ItemSpawn->CreateWeapon(int32(EWeaponType::Gun), GetActorLocation() + FVector(0, 50, 50), GetActorRotation());
			UE_LOG(LogTemp, Error, TEXT("Player Gun Drop!")); //확인용 텍스트 출력

			isPlayerVisibleGun = false;
		}
		else if (isPlayerVisibleLollipop == true) //만약 플레이어의 롤리팝이 보이면(플레이어가 롤리팝을 들고 있을 때)
		{	
			compMeshLollipop->SetVisibility(false); //플레이어의 총 컴포넌트가 안보이게한다
			//GetWorld()->SpawnActor<ALollipopWeapon>(weaponLollipop, GetActorLocation() + FVector(-200, 0, 0), GetActorRotation()); //LollipopWeapon이 플레이어 위치의 바닥에 스폰된다
			ItemSpawn->CreateWeapon(int32(EWeaponType::Lollipop), GetActorLocation() + FVector(0, 50, 50), GetActorRotation());
			UE_LOG(LogTemp, Error, TEXT("Player Lollipop Drop!")); //확인용 텍스트 출력

			isPlayerVisibleLollipop = false;
		}
}

		

//############### 플레이어 HP, 데미지 ###############
//[플레이어 HP 감소 함소] 플레이어가 공격 당하면 에너미에서 호출하는 함수. 플레이어의 데미지니까 플레이어에서 구현
void ARIM_Player::OnDamageProcess()
{
	HP--; //플레이어 HP 1씩 감소
	UE_LOG(LogTemp, Error, TEXT("%d"),HP); //확인용 텍스트 출력
}

//[플레이어 데미지, 넉다운 함수] 플레이어가 공격 당하면 에너미에서 호출하는 함수. 플레이어의 데미지니까 플레이어에서 구현
void ARIM_Player::DamagePlay()
{
	if (isplayerDown == false) //서있을 때
	{
		if (HP > 0)
		{
			//플레이어 애니메이션 몽타주 중 '데미지' 애니메이션 랜덤 재생
			rand = FMath::RandRange(0, 2);
			animPlayer->PlayPlayerTwoAnim(TEXT("Damaged"), rand);

			InputDropWeapon();
		}
		else if (HP <= 0) //만약 에너미한테 5번 공격 받으면(HP가 5이고 공격 받을때마다 HP가 1씩 줄어드니까 5번 공격 받는 것은 HP가 0이 되는 것과 동일하다)
		{
			//플레이어 애니메이션 몽타주 중 '넉다운' 애니메이션 랜덤 재생
			rand = FMath::RandRange(0, 1);
			animPlayer->PlayPlayerTwoAnim(TEXT("Knockdown"), rand);

			UE_LOG(LogTemp, Error, TEXT("Player Knockdown!")); //확인용 텍스트 출력

			//넉다운 할 때 무기 버리기 ★★★inputDropWeapon 은 F 버튼 눌렀을 때 무기 버리는 건데, 버튼 안 눌러도 버려지게 해야하는데 뭔가 더 필요해 보이는 느낌...
			InputDropWeapon();

			//플레이어 누움. 기절 함
			isplayerDown = true;

			//5초 후 일어나는 코드는 Tick에서 구현한다. 시간이 흘러야 하니까
		}
	}
}

//[플레이어 죽음]
void ARIM_Player::Die()
{
		//플레이어 애니메이션 몽타주 중 '죽음' 애니메이션 랜덤 재생
		rand = FMath::RandRange(0, 1);
		animPlayer->PlayPlayerTwoAnim(TEXT("Die"), rand);
		
		ADogFightGameModeBase* gamemodefail = GetWorld()->GetAuthGameMode<ADogFightGameModeBase>(); //가져온다
		gamemodefail->addtoViewfail(); //함수 호출

		isplayerDown = true; //플레이어 누웠다. 키 눌렀을 때 안 움직이게 하기 위해 추가 함
		isplayerDie = true; //플레이어 죽었다
}



//int a = 10;
//int* b = &a;
//*b = 20;