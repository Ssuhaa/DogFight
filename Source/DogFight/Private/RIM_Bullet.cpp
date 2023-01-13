// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Bullet.h"
#include <Components/SphereComponent.h> //USphereComponent 를 사용하기 위해 추가
#include <GameFramework/ProjectileMovementComponent.h> //UProjectileMovementComponentf 를 사용하기 위해 추가
#include "SH_Enemy.h"
#include "SH_EnemyFSM.h"

// Sets default values
ARIM_Bullet::ARIM_Bullet() //생성자
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[총알 충돌체 컴포넌트 추가]
	//충돌체 등록하기. 충돌체 인스턴스 등록하고 compCollision 변수에 담기
	//충돌체를 루트 컴포넌트로 등록
	//충돌체 크기 설정
	//충돌프로파일 설정. 충돌체의 충돌 프로파일/프리셋을 Weapon 으로 지정
	compCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = compCollision;
	compCollision->SetSphereRadius(40); //▶추후 필요시 변경
	compCollision->SetCollisionProfileName(TEXT("Weapon"));
	


	//[총알 스테틱메시 컴포넌트 추가]
	//외관 컴포넌트 등록하기. UStaticMeshComponent 인스턴스 만들고 compMeshWeaponBullet 멤버 변수에 할당
	//부모 컴포넌트 지정. 부모 컴포넌트로 coolisionComp 지정
	//외관 크기 설정
	//충돌 비활성화
	compMeshWeaponBullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponBulletMeshComp"));
	compMeshWeaponBullet->SetupAttachment(compCollision);
	compMeshWeaponBullet->SetRelativeLocation(FVector(10.260604f, 0, -28.190779f));
	compMeshWeaponBullet->SetRelativeScale3D(FVector(0.25f)); //▶추후 필요시 변경. 블루프린트에서 조정 시 삭제
	compMeshWeaponBullet->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//[총알 스테틱메시. 옷 추가]
	ConstructorHelpers::FObjectFinder <UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (tempMesh.Succeeded())
	{
		compMeshWeaponBullet->SetStaticMesh(tempMesh.Object);
	}



	//[총알 발사체 컴포넌트 추가]
	//발사체 컴포넌트
	//movement 컴포넌트가 생산시킬 컴포넌트 지정
	compMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp")); //★★★???
	compMovement->SetUpdatedComponent(compCollision); //★★★???
	


	//[총알 발사체 컴포넌트 초기값 설정] //★★★전체 필요 없을 시 삭제
	//초기속도. InitialSpeed 속성 이용
	//최대속도. MaxSpeed 속성 이용
	//반동여부. bShouldBounce 속성 이용. true 할당
	//반동 값. 반동이 있다면 탄성은 어느 정도 될지 Bounciness 속성 이용
	//생명 시간 주기
	compMovement->InitialSpeed = 2000; //▶추후 필요시 변경
	compMovement->MaxSpeed = 5000; //▶추후 필요시 변경
	compMovement->bShouldBounce = true;
	compMovement->Bounciness = 0.2f; //▶추후 필요시 변경
	InitialLifeSpan = 1.0f; //▶추후 필요시 변경
}

// Called when the game starts or when spawned
void ARIM_Bullet::BeginPlay()
{
	Super::BeginPlay();

	//[[1]총알 콜리전과 에너미가 충돌 시 실행 될 함수 호출] ★★★완벽하게 이해하지 못 함. 왜 BeginPlay에서 하는지???
	compCollision->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Bullet::collisionBulletBeginOverlap);


}

// Called every frame
void ARIM_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//[[3]BeginOverlap 총알 콜리전과 에너미가 충돌할 때 실행할 내용 / 함수 구현)]
void ARIM_Bullet::collisionBulletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains(TEXT("Enemy"))) //만약에 부딛힌 OtherActor 중에 이름이 Enemy 를 포함하고있으면
	{
		UE_LOG(LogTemp, Error, TEXT("Player Fire Attack!!!")); //확인용 텍스트 출력

		//[에너미에 데미지가 들어간다 함수 호출]
		Enemy = Cast<ASH_Enemy>(OtherActor); //에너미 형변환! 했을 때 널인지 아닌지 꼭 체크해야 한다
		                                     //부딛힌 OtherActor(이름 중에 Enemy 를 포함하고 있는)가 SH_Enemy 클래스의 Enemy 이면 Enemy 라고 한다
		
		if (Enemy != nullptr) //에너미가 아니다 = 에너미이다. 플레이어 총알에 맞은 것이 에너미가 아닐 수도 있기 때문에 에너미인지 확인 필요
		{
			//에너미가 대기, 이동, 공격, 무기 들기 상태 일 때
			if (Enemy->fsm->mState == EEnemyState::Idle || Enemy->fsm->mState == EEnemyState::Move || Enemy->fsm->mState == EEnemyState::Attack || Enemy->fsm->mState == EEnemyState::Pickup)
			{
				Enemy->fsm->OnDamageProcess(); //SH_Enemy 클래스 Enemy(액터 상속 받음)에서 fsm(액터 컴포넌트 상속 받음)에서 데미지프로세스 호출
				Destroy(); //총알 콜리전이 에너미와 충돌하면 파괴 된다
			}
		}
	}
}