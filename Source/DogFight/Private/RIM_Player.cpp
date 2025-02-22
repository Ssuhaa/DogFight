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
#include <Kismet/KismetMathLibrary.h>
#include "ShovelWeapon.h"
#include "TennisWeapon.h"
#include "FogManager.h"

// Sets default values
ARIM_Player::ARIM_Player() //생성자
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[플레이어 스켈레탈메시 등록. 세팅]
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor2.Charactor2'")); 
	if (tempMesh.Succeeded()) 
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object); 
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0)); 
	GetMesh()->SetCollisionProfileName(TEXT("Player")); //피짘스 사용해서 넣어야 함

	//[플레이어 본체 충돌 컴포넌트 Collision 세팅]
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player")); //콜리전 프리셋을 Player 로 설정. Player 프리셋 엔진에서 만듬 

	//[스프링암 컴포넌트 추가. USpringArmComponent]
	compSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp")); 
	compSpringArm->SetupAttachment(RootComponent); //루트컴포넌트에 자식으로 붙임
	compSpringArm->TargetArmLength = 900; //▶추후 필요시 변경
	compSpringArm->SocketOffset = FVector(0, 0, 450); //▶추후 필요시 변경
	compSpringArm->bUsePawnControlRotation = true; //입력에 따른 회전 설정
	compSpringArm->bDoCollisionTest = false; //스프링암 카메라 콜리전 끔. 캐릭터들끼리 뭉쳐 보이는 현상 방지

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
	compMeshGun->SetupAttachment(GetMesh(), TEXT("Gun")); //부모 컴포넌트를 Mesh 컴포넌트로 설정. 총을 캐릭터 손 소켓에 붙임.
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempGunMesh(TEXT("StaticMesh'/Game/Geometry/Gun.Gun'"));
	if (tempGunMesh.Succeeded())
	{
		compMeshGun->SetStaticMesh(tempGunMesh.Object);
	}
	compMeshGun->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//[롤리팝 스태틱메시 컴포넌트 추가]
	compMeshLollipop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LollipopMeshComp"));
	compMeshLollipop->SetupAttachment(GetMesh(), TEXT("Lollipop")); //부모 컴포넌트를 Mesh 컴포넌트로 설정. 롤리팝을 캐릭터 손 소켓에 붙임.
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempLollipopMesh(TEXT("StaticMesh'/Game/Geometry/Lollipop.Lollipop'"));
	if (tempLollipopMesh.Succeeded())
	{
		compMeshLollipop->SetStaticMesh(tempLollipopMesh.Object);
	}
	compMeshLollipop->SetRelativeLocation(FVector(0.686449f, -9.618860f, 2.343055f));
	compMeshLollipop->SetRelativeRotation(FRotator(0, 0, -20));
	compMeshLollipop->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//[삽 스태틱메시 컴포넌트 추가]
	compMeshShovel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShovelMeshComp"));
	compMeshShovel->SetupAttachment(GetMesh(), TEXT("Shovel"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempShoveMesh(TEXT("StaticMesh'/Game/Geometry/SM_shovel.SM_shovel'"));
	if (tempShoveMesh.Succeeded())
	{
		compMeshShovel->SetStaticMesh(tempShoveMesh.Object);
	}
	compMeshShovel->SetRelativeLocation(FVector(0, 4, -5));
	compMeshShovel->SetRelativeRotation(FRotator(3, 170, 89));
	//compMeshShovel->SetRelativeScale3D(FVector(0.8f));
	compMeshShovel->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//[테니스라켓 스태틱메시 컴포넌트 추가]
	compMeshTennis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TennisMeshComp"));
	compMeshTennis->SetupAttachment(GetMesh(), TEXT("Tennis"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempTennisMesh(TEXT("StaticMesh'/Game/Geometry/Tenis.Tenis'"));
	if (tempTennisMesh.Succeeded())
	{
		compMeshTennis->SetStaticMesh(tempTennisMesh.Object);
	}
	compMeshTennis->SetRelativeLocation(FVector(-4, -2, 0));
	compMeshTennis->SetRelativeRotation(FRotator(34, -73, -68));
	compMeshTennis->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//[불렛 클래스 가져와서 등록. 불렛팩토리 안에 불렛 디폴트로 추가] <불렛을 변수에 담아 놓고 내가 원하는 때에 스폰하려고 변수로 담아 놓는 것>
	ConstructorHelpers::FClassFinder<ARIM_Bullet> tempBul(TEXT("Blueprint'/Game/BluePrint/BP_RIM_Bullet.BP_RIM_Bullet_C'"));
	if (tempBul.Succeeded())
	{
		bulletFactory = tempBul.Class;
	}

	//[무기 드랍할때 스폰할 총 클래스 가져와서 등록]
	ConstructorHelpers::FClassFinder<AGunWeapon> tempGun(TEXT("Class'/Script/DogFight.GunWeapon'"));
	if (tempGun.Succeeded())
	{
		weaponGun = tempGun.Class;
	}

	//[무기 드랍할때 스폰할 롤리팝 클래스 가져와서 등록]
	ConstructorHelpers::FClassFinder<ALollipopWeapon> tempLollipop(TEXT("Class'/Script/DogFight.LollipopWeapon'"));
	if (tempLollipop.Succeeded())
	{
		weaponLollipop = tempLollipop.Class;
	}

	//[무기 드랍할 때 스폰할 삽 클래스 가져와서 등록]
	ConstructorHelpers::FClassFinder<AShovelWeapon> tempShovel(TEXT("Class'/Script/DogFight.ShovelWeapon'"));
	if (tempShovel.Succeeded())
	{
		weaponShovel = tempShovel.Class;
	}

	//[무기 드랍할때 스폰할 테니스라켓 가져와서 등록]
	ConstructorHelpers::FClassFinder<ATennisWeapon> tempTennis(TEXT("Class'/Script/DogFight.TennisWeapon'"));
	if (tempTennis.Succeeded())
	{
		weaponTennis = tempTennis.Class;
	}

	//[플레이어 ABP_애니메이션 클래스 가져와서 등록]
	ConstructorHelpers::FClassFinder<URIM_PlayerAnim> tempAnim(TEXT("AnimBlueprint'/Game/BluePrint/ABP_Player.ABP_Player_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class); //플레이어->메시->애니메이션->애님클래스 에 추가. Anim Class 에 드롭다운으로 ABP_Player 추가
	}

	PhysicComp = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnim"));

	DeadBlock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DeadBlock"));
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempCube(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempCube.Succeeded())
	{
		DeadBlock->SetStaticMesh(tempCube.Object);
	}
	DeadBlock->SetupAttachment(GetMesh());
	DeadBlock->SetRelativeLocation(FVector(0, -30, 170));
	DeadBlock->SetRelativeScale3D(FVector(0.75f, 1.5f, 2.0f));
	DeadBlock->SetCollisionResponseToAllChannels(ECR_Ignore);
	DeadBlock->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Block);
	DeadBlock->SetVisibility(false);

	GetCharacterMovement()->JumpZVelocity = jumpZ;
}

// Called when the game starts or when spawned
void ARIM_Player::BeginPlay()
{
	Super::BeginPlay();

	//[초기 속도를 걷기로 설정]
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	//[총이 시작할때 안보이게 설정]
	compMeshGun->SetVisibility(false);

	//[롤리팝이 시작할때 안 보이게 설정]
	compMeshLollipop->SetVisibility(false);
	
	//[삽이 시작할때 안 보이게 설정]
	compMeshShovel->SetVisibility(false);

	//[테니스라켓 시작할때 안 보이게 설정]
	compMeshTennis->SetVisibility(false);

	//애니메이션 관련 <메쉬에 선택된 애니메이션을 변수에 담아 놓는 것> ★★★아직 이해 안됨
	animPlayer = Cast<URIM_PlayerAnim>(GetMesh()->GetAnimInstance());

	//[월드에 스폰된 모든 에너미 어레이에 등록] ★★★왜 필요?
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASH_Enemy::StaticClass(), enemyActor);
	for (int32 i = 0; i < enemyActor.Num(); i++)
	{
		ASH_Enemy* currentenemy = Cast <ASH_Enemy>(enemyActor[i]);
		enemyarray.Add(currentenemy);
	}

	setPhysicsData();
}

void ARIM_Player::setPhysicsData()
{

	FPhysicalAnimationData PhysicalAnimDate;
	PhysicalAnimDate.bIsLocalSimulation = true;
	PhysicComp->StrengthMultiplyer = 1.0f;
	PhysicalAnimDate.OrientationStrength = 5000.0f;
	PhysicalAnimDate.AngularVelocityStrength = 100.0f;
	PhysicalAnimDate.PositionStrength = 5000.0f;
	PhysicalAnimDate.VelocityStrength = 100.0f;
	PhysicalAnimDate.MaxAngularForce = 100.0f;
	PhysicalAnimDate.MaxLinearForce = 100.0f;

	PhysicComp->SetSkeletalMeshComponent(GetMesh());
	PhysicComp->ApplyPhysicalAnimationSettingsBelow(TEXT("spine_01"), PhysicalAnimDate, true);
	GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("spine_01"), true);
}

// Called every frame
void ARIM_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//[Move(플레이어 이동) 함수 호출]
	Move();

	//[플레이어 넉다운(기절) 후 일어나는 코드]
	if (playerState == EPlayerState::KnockDown) //플레이어가 누워있으면
	{
		currentTime += DeltaTime; //현재시간 + 델타타임 = 현재 누적시간

		if (currentTime > 5) //플레이어가 기절한 시간(5초)가 지나면
		{
			//플레이어 애니메이션 몽타주 중 '업' 애니메이션 랜덤 재생
			rand = FMath::RandRange(0, 1);
			animPlayer->PlayPlayerTwoAnim(TEXT("WakeUp"), rand);

			UE_LOG(LogTemp, Error, TEXT("Player Wake up!")); //확인용 텍스트 출력

			currentTime = 0; //현재 시간을 0초로 초기화 한다
			HP = 5; //플레이어의 HP를 5초로 초기화 한다

			//isplayerDown = false;//플레이어가 쓰러져있는지 확인하는 변수 초기화. 플레이어 일어난다 
			playerState = EPlayerState::Idle;
		}
	}

	//[실패 위젯 띄운다]
	if (playerState == EPlayerState::Die) //플레이어가 죽은 상태일 때
	{
		currentTime += DeltaTime;

		if (currentTime > 2) //2초 후에
		{
			UGameplayStatics::OpenLevel(GetWorld(), "Result"); //결과 레벨(화면)을 연다
		}
	}

	if (GetVelocity().Length() > 650) //벨로시티가 650이상이면 강제로 600으로 맞춰준다.
	{
		FVector v = GetVelocity().GetSafeNormal();
		GetCharacterMovement()->Velocity = v * 600;
	}
	if (playerState == EPlayerState::KnockDown)
	{
		GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("spine_01"), false);
		DeadBlock->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("spine_01"), true);
		DeadBlock->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &ARIM_Player::InputRun); //★★★ 왜 필요?

	//[공격/잡기 이벤트 처리 함수 바인딩/호출] = 총알 발사 + 펀치 + 무기 공격
	PlayerInputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, &ARIM_Player::InputPunchGrab);

	//[드롭킥/던지기 이벤트 처리 함수 바인딩/호출]
	PlayerInputComponent->BindAction(TEXT("KickToss"), IE_Pressed, this, &ARIM_Player::InputKickToss);

	//[박치기 이벤트 처리 함수 바인딩/호출]
	PlayerInputComponent->BindAction(TEXT("Headbutt"), IE_Pressed, this, &ARIM_Player::InputHeadbutt);

	//[무기 버리기 이벤트 처리 함수 바인딩/호출]
	PlayerInputComponent->BindAction(TEXT("DropWeapon"), IE_Pressed, this, &ARIM_Player::InputDropWeapon);
}

//[Move(플레이어 이동) 함수 구현]
void ARIM_Player::Move()
{
	//if (isplayerDown == true || isplayerDie == true)  return; //플레이어가 살아있을 때 만 이동 가능하게 한다. 플레이어가 누워 있거나, 죽어있으면 Move 함수를 실행하지 않는다.
	if (playerState == EPlayerState::KnockDown || playerState == EPlayerState::Die) return;

	//[플레이어 이동 구현 코드]
	direction = FTransform(GetControlRotation()).TransformVector(direction); //이동 방향을 컨트롤 방향 기준으로 변환
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
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
	if (playerState == EPlayerState::KnockDown || playerState == EPlayerState::Die) return;
	{
		Jump();
	}
}


//[공격 이벤트 처리 함수 구현] = 총알 발사 + 펀치 + 무기 공격
void ARIM_Player::InputPunchGrab()
{
	//if (isplayerDown == true || isplayerDie == true)  return; //플레이어가 살아있을때만 공격할 수 있게 한다. 플레이어가 누워있거나, 죽어있으면 InputPunchGrab 함수를 실행하지 않는다.
	if (playerState == EPlayerState::KnockDown || playerState == EPlayerState::Die) return;

	if (compMeshGun->IsVisible()) //플레이어가 총을 들고 있으면 -> 총에서 총알을 발사한다
	{
		animPlayer->PlayPlayerAnim(TEXT("Fire"), 0); //플레이어 애니메이션 몽타주 중 '파이어' 애니메이션 재생
		GetWorld()->SpawnActor<ARIM_Bullet>(bulletFactory, GetActorLocation() + GetActorForwardVector() * 200, GetActorRotation()); //총알 스폰
	}
	else if (compMeshLollipop->IsVisible()) //플레이어가 총을 들고 있지 않고, 롤리팝을 들고 있으면 -> 롤리팝으로 공격한다
 	{
		animPlayer->PlayPlayerTwoAnim(TEXT("Lollipop"), 0); //플레이어 애니메이션 몽타주 중 '롤리팝' 애니메이션 재생
		TargetDotAttack();
	}
	else if (compMeshShovel->IsVisible()) //플레이어가 총, 롤리팝을 들고 있지 않고, 삽을 들고 있으면 -> 삽으로 공격한다
	{
		animPlayer->PlayPlayerTwoAnim(TEXT("Lollipop"), 0); //플레이어 애니메이션 몽타주 중 '삽' 애니메이션 재생
		TargetDotAttack();
	}
	else if (compMeshTennis->IsVisible()) //플레이어가 총, 롤리팝, 삽을 들고 있지 않고, 테니스 라켓을 들고 있으면 -> 테니스 라켓으로 공격한다
	{ 
		animPlayer->PlayPlayerAnim(TEXT("Swing"), 0); //플레이어 애니메이션 몽타주 중 '테니스' 애니메이션 재생
		TargetDotAttack();
	}
	else //플레이어가 총, 롤리팝, 삽 들고 있지 않고 InputPunchGrab 버튼을 누르면 -> 펀치한다 
	{
		animPlayer->PlayPlayerAnim(TEXT("Swing"), 0); //플레이어 애니메이션 몽타주 중 '펀치' 애니메이션 재생
		TargetDotAttack();
	}
}


//############### 킥 ###############
// [드롭킥/던지기 이벤트 처리 함수 구현]
void ARIM_Player::InputKickToss()
{
	//if (isplayerDown == true || isplayerDie == true)  return; //플레이어가 살아있을때만 공격할 수 있게 한다. 플레이어가 누워있거나, 죽어있으면 InputKickToss 함수를 실행하지 않는다.
	if (playerState == EPlayerState::KnockDown || playerState == EPlayerState::Die) return;

	animPlayer->PlayPlayerTwoAnim(TEXT("Kick"), 0); //플레이어 애니메이션 몽타주 중 '킥' 애니메이션 재생
	TargetDotAttack();

	GetCharacterMovement()->SetActive(false);
}


//############### 박치기 ###############
// [박치기 이벤트 처리 함수 구현]
void ARIM_Player::InputHeadbutt() 
{
	//if (isplayerDown == true || isplayerDie == true)  return; //플레이어가 살아있을때만 공격할 수 있게 한다. 플레이어가 누워있거나, 죽어있으면 InputKickToss 함수를 실행하지 않는다.
	if (playerState == EPlayerState::KnockDown || playerState == EPlayerState::Die) return;

	animPlayer->PlayPlayerAnim(TEXT("Headbutt"), 0); //플레이어 애니메이션 몽타주 중 '헤딩' 애니메이션 재생
	TargetDotAttack();
}


//[플레이어에 총이 보이게 하는 함수 구현]
void ARIM_Player::VisibleGun()
{
	if (compMeshLollipop->IsVisible() || compMeshShovel->IsVisible() || compMeshTennis->IsVisible()) return; //롤리팝 or 삽 or 테니스라켓을 들고 있을 때 총 못 들게 한다.
	compMeshGun->SetVisibility(true); //플레이어가 든 총이 보이게 한다
}

//[플레이어에 롤리팝이 보이게 하는 함수 구현]
void ARIM_Player::VisibleLollipop()
{
	if (compMeshGun->IsVisible() || compMeshShovel->IsVisible() || compMeshTennis->IsVisible()) return; //총 or 삽 or 테니스라켓을 들고 있을 때 롤리팝 못 들게 한다.
	compMeshLollipop->SetVisibility(true);
}

//[플레이어에 삽이 보이게 하는 함수 구현]
void ARIM_Player::VisibleShovel()
{
	if (compMeshGun->IsVisible() || compMeshLollipop->IsVisible() || compMeshTennis->IsVisible()) return; //총 or 롤리팝 or 테니스라켓을 들고 있을 때 삽 못 들게 한다.
	compMeshShovel->SetVisibility(true);
}

//[플레이어에 테니스가 보이게 하는 함수 구현]
void ARIM_Player::VisibleTennis()
{
	if (compMeshGun->IsVisible() || compMeshLollipop->IsVisible() || compMeshShovel->IsVisible()) return; //총 or 롤리팝 or 삽을 들고 있을 때 테니스라켓을 못 들게 한다.
	compMeshTennis->SetVisibility(true);
}


//[무기 버리기 이벤트 처리 함수 구현]
void ARIM_Player::InputDropWeapon()
{
	AItemSpawn* ItemSpawn = Cast<AItemSpawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AItemSpawn::StaticClass())); //★★★???
	if (compMeshGun->IsVisible()) //플레이어가 총을 들고 있으면
	{
		ItemSpawn->CreateWeapon(int32(EWeaponType::Gun), GetActorLocation() + FVector(0, 50, 50), GetActorRotation()); //GunWeapon이 플레이어 위치의 바닥에 스폰된다. //★★★어떤 원리???
		compMeshGun->SetVisibility(false); //플레이어의 총 컴포넌트가 안보이게한다
		UE_LOG(LogTemp, Error, TEXT("Player Gun Drop!")); //확인용 텍스트 출력
	}
	else if (compMeshLollipop->IsVisible()) //플레이어가 롤리팝을 들고 있으면
	{
		ItemSpawn->CreateWeapon(int32(EWeaponType::Lollipop), GetActorLocation() + FVector(0, 50, 50), GetActorRotation()); //★★★어떤 원리???
		compMeshLollipop->SetVisibility(false);
		UE_LOG(LogTemp, Error, TEXT("Player Lollipop Drop!"));
	}
	else if (compMeshShovel->IsVisible()) //플레이어가 삽을 들고 있으면
	{
		ItemSpawn->CreateWeapon(int32(EWeaponType::Shovel), GetActorLocation() + FVector(0, 50, 50), GetActorRotation()); //★★★어떤 원리???
		compMeshShovel->SetVisibility(false);
		UE_LOG(LogTemp, Error, TEXT("Player Shovel Drop!"));
	}
	else if (compMeshTennis->IsVisible()) //플레이어가 테니스라켓을 들고 있으면
	{
		ItemSpawn->CreateWeapon(int32(EWeaponType::Tennis), GetActorLocation() + FVector(0, 50, 50), GetActorRotation()); //★★★어떤 원리???
		compMeshTennis->SetVisibility(false);
		UE_LOG(LogTemp, Error, TEXT("Player Tennis Drop!"));
	}
}


//[플레이어 HP 감소 함소] 플레이어가 공격 당하면 에너미에서 호출하는 함수. 플레이어의 데미지니까 플레이어에서 구현
void ARIM_Player::OnDamageProcess()
{
	HP--; //플레이어 HP 1씩 감소
	UE_LOG(LogTemp, Error, TEXT("%d"), HP); //확인용 텍스트 출
	InputDropWeapon();
	if (HP > 0)
	{
		//플레이어 애니메이션 몽타주 중 '데미지' 애니메이션 랜덤 재생
		rand = FMath::RandRange(0, 2);
		animPlayer->PlayPlayerTwoAnim(TEXT("Damaged"), rand);
		UE_LOG(LogTemp, Error, TEXT("Player Damaged!")); //확인용 텍스트 출력
	}
	else if (HP <= 0) //만약 에너미한테 5번 공격 받으면(HP가 5이고 공격 받을때마다 HP가 1씩 줄어드니까 5번 공격 받는 것은 HP가 0이 되는 것과 동일하다)
	{
		//플레이어 애니메이션 몽타주 중 '넉다운' 애니메이션 랜덤 재생
		rand = FMath::RandRange(0, 1);
		animPlayer->PlayPlayerTwoAnim(TEXT("Knockdown"), rand); //5초 후 일어나는 코드는 Tick에서 구현한다. 시간이 흘러야 하니까
		//플레이어 누움. 기절 함
		//isplayerDown = true;
		playerState = EPlayerState::KnockDown;

		UE_LOG(LogTemp, Error, TEXT("Player Knockdown!")); //확인용 텍스트 출력
	}
}


//[플레이어 죽음]
void ARIM_Player::Die()
{
	//플레이어 애니메이션 몽타주 중 '죽음' 애니메이션 랜덤 재생
	rand = FMath::RandRange(0, 1);
	animPlayer->PlayPlayerTwoAnim(TEXT("Die"), rand);

	ADogFightGameModeBase* gamemodefail = GetWorld()->GetAuthGameMode<ADogFightGameModeBase>(); //게임모드를 가져온다.
	gamemodefail->addtoViewfail(); //게임모드의 Fail 위젯 띄우는 함수를 호출한다.

	//isplayerDie = true; //플레이어 죽음 
	playerState = EPlayerState::Die;
}


//[도트. 플레이어의 시야안에서 보이는 에너미의 거리를 재서 가까우면 맞게하는 함수] ★★★★★
void ARIM_Player::TargetDotAttack()
{
	for (int32 i = 0; i < enemyarray.Num(); i++)
	{
		FVector dir = enemyarray[i]->GetActorLocation() - GetActorLocation(); // 방향. 플레이어에서 에너미를 향하는 방향 구하기
		float dotValue = FVector::DotProduct(GetActorForwardVector(), dir.GetSafeNormal()); //내적. 플레이어의 앞방향과 위에서 구한 벡터(노멀라이즈 필요)의 내적
		float angle = UKismetMathLibrary::DegAcos(dotValue); //내적각도 구하기. 위에서 나온 값을 Acos 하기
		if (angle < 30 && dir.Length() < 300) //플레이어 시야 60도 사이에 에너미 거리가 300이면
		{	
			if (enemyarray[i]->fsm->mState != EEnemyState::Down && enemyarray[i]->fsm->mState != EEnemyState::Die && enemyarray[i]->fsm->mState != EEnemyState::Damage) //에너미의 상태를 확인하고(넉다운+죽음+데미지 상태가 아니면)
			{
				enemyarray[i]->fsm->OnDamageProcess(); //에너미에게 데미지를 준다.
			}
		}
	}
}