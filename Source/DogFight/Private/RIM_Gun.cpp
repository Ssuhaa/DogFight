// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Gun.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "RIM_Player.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ARIM_Gun::ARIM_Gun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[충돌체 컴포넌트 추가]
	//충돌체 등록하기. 충돌체 인스턴스 등록하고 collisionComp 변수에 담기
	//충돌체를 루트 컴포넌트로 등록
	//충돌프로파일 설정. 충돌체의 충돌 프로파일을 BlockAll로 지정. 모든 물체와 부딪혀 튕귈 수 있게 하기
	collisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = collisionComp;
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));



	//[스테틱메시 컴포넌트 추가]
	//외관 컴포넌트 등록하기. UStaticMeshComponent 인스턴스 만들고 bodyMeshComp 멤버 변수에 할당
	//부모 컴포넌트 지정. 부모 컴포넌트로 coolisionComp 지정
	//충돌 비활성화
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("itemGunMeshComp"));
	bodyMeshComp->SetupAttachment(collisionComp);
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//ConstructorHelpers::FObjectFinder<UStaticMesh> tempmesh(TEXT(""));
// 	if (tempmesh.Succeeded())
// 	{
// 		bodyMeshComp->Set
// 	}


	//[movementComp 컴포넌트 추가] //★★★필요 없을 시 전체 삭제
	//movementComp 컴포넌트
	//movement 컴포넌트가 생산시킬 컴포넌트 지정
	//movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	//movementComp->SetUpdatedComponent(collisionComp);

	//[movementComp 컴포넌트 초기값 설정] //★★★필요 없을 시 전체 삭제
	//초기속도. InitialSpeed 속성 이용
	//최대속도. MaxSpeed 속성 이용
	//반동여부. bShouldBounce 속성 이용. true 할당
	//반동 값. 반동이 있다면 탄성은 어느 정도 될지 Bounciness 속성 이용
	//생명 시간 주기
	//movementComp->InitialSpeed = 500; //★★★추후 조정 필요
	//movementComp->MaxSpeed = 500; //★★★추후 조정 필요
	//movementComp->bShouldBounce = true;
	//movementComp->Bounciness = 0.2f; //★★★추후 조정 필요
	//InitialLifeSpan = 30.0f; //★★★추후 조정 필요


	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Gun::collisionBeginOverlap);
	collisionComp->OnComponentEndOverlap.AddDynamic(this, &ARIM_Gun::collisionEndOverlap);
	
	

}

// Called when the game starts or when spawned
void ARIM_Gun::BeginPlay()
{
	Super::BeginPlay();
	//[바닥에 있는 총과 플레이어 충돌 이벤트 함수 바인딩/호출]

}

// Called every frame
void ARIM_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARIM_Gun::EnableInput(APlayerController* PlayerController)
{
	Super::EnableInput(PlayerController);

	PlayerController->InputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, &ARIM_Gun::getGun);
}


//[바닥에 있는 총과 플레이어 충돌 이벤트 함수 구현]
void ARIM_Gun::collisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("11111111111111"));
	RIM_Player = Cast<ARIM_Player>(OtherActor);
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
}

void ARIM_Gun::collisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//AActor* RIM_Player = Cast<ARIM_Player>(OtherActor);
	//DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ARIM_Gun::getGun()
{
	UE_LOG(LogTemp, Warning, TEXT("22222222"));
	RIM_Player->VisibleGun();
	Destroy();
}

void ARIM_Gun::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("5555555555"));
	RIM_Player = Cast<ARIM_Player>(OtherActor);
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ARIM_Gun::NotifyActorEndOverlap(AActor* OtherActor)
{
	AActor* RIM_Player = Cast<ARIM_Player>(OtherActor);
	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}