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

	//[바닥에 있는 총 충돌 컴포넌트(충돌체) 추가]
	//충돌체 등록하기. 충돌체 인스턴스 등록하고 compCollision 변수에 담기
	//충돌체를 루트 컴포넌트로 등록
	//충돌프로파일 설정. 충돌체의 충돌 프로파일을 BlockAll로 지정. 모든 물체와 부딪혀 튕귈 수 있게 하기
	compCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = compCollision;
	compCollision->SetCollisionProfileName(TEXT("BlockAll"));



	//[바닥에 있는 총 스테틱메시 컴포넌트 추가]
	//외관 컴포넌트 등록하기. UStaticMeshComponent 인스턴스 만들고 compMeshWeaponGun 멤버 변수에 할당
	//부모 컴포넌트 지정. 부모 컴포넌트로 coolisionComp 지정
	//충돌 비활성화
	compMeshWeaponGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponGunMeshComp"));
	compMeshWeaponGun->SetupAttachment(compCollision);
	compMeshWeaponGun->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//[바닥에 있는 총 스테틱메시. 옷 추가]
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempItemGunMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'")); 
	if (TempItemGunMesh.Succeeded())
	{
		compMeshWeaponGun->SetStaticMesh(TempItemGunMesh.Object);
		compMeshWeaponGun->SetRelativeLocationAndRotation(FVector(0, 0, -20), FRotator(0, 0, 0));
		compMeshWeaponGun->SetRelativeScale3D(FVector(0.35f));
	}

}

// Called when the game starts or when spawned
void ARIM_Gun::BeginPlay()
{
	Super::BeginPlay();
	
	//[바닥에 있는 총과 플레이어 충돌 시...?]
	compCollision->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Gun::collisionBeginOverlap);
	compCollision->OnComponentEndOverlap.AddDynamic(this, &ARIM_Gun::collisionEndOverlap);


}

// Called every frame
void ARIM_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

//[???]
void ARIM_Gun::EnableInput(APlayerController* PlayerController)
{
	Super::EnableInput(PlayerController);

	PlayerController->InputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, &ARIM_Gun::getGun);
}




//[바닥에 있는 총과 플레이어 충돌 시 함수 구현]
void ARIM_Gun::collisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //원래 충돌한게 뭔지 확인하는거 있어야한다
	RIM_Player = Cast<ARIM_Player>(OtherActor);
	if (RIM_Player != nullptr)
	{
		EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
}

void ARIM_Gun::collisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	RIM_Player = Cast<ARIM_Player>(OtherActor);
}




//[바닥에 있는 총 안 보이게 하는 함수 구현]
void ARIM_Gun::getGun()
{
	//플레이어가 들고 있는 총이 보이면
	//바닥에 있는 총 파괴(안 보임)
	RIM_Player->VisibleGun();
	Destroy();
}

/*
void ARIM_Gun::NotifyActorBeginOverlap(AActor* OtherActor)
{
	RIM_Player = Cast<ARIM_Player>(OtherActor);
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ARIM_Gun::NotifyActorEndOverlap(AActor* OtherActor)
{
	AActor::EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}
*/