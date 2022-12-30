// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Bullet.h"
#include <Components/SphereComponent.h> //USphereComponent 를 사용하기 위해 추가
#include <GameFramework/ProjectileMovementComponent.h> //UProjectileMovementComponentf 를 사용하기 위해 추가

// Sets default values
ARIM_Bullet::ARIM_Bullet() //생성자
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[총알 충돌체 컴포넌트 추가]
	//충돌체 등록하기. 충돌체 인스턴스 등록하고 compCollision 변수에 담기
	//충돌체를 루트 컴포넌트로 등록
	//충돌체 크기 설정
	//충돌프로파일 설정. 충돌체의 충돌 프로파일을 BlockAll로 지정. 모든 물체와 부딪혀 튕귈 수 있게 하기
	compCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = compCollision;
	compCollision->SetSphereRadius(13); //★★★추후 조정 필요
	compCollision->SetCollisionProfileName(TEXT("BlockAll"));
	


	//[총알 스테틱메시 컴포넌트 추가]
	//외관 컴포넌트 등록하기. UStaticMeshComponent 인스턴스 만들고 compMeshWeaponBullet 멤버 변수에 할당
	//부모 컴포넌트 지정. 부모 컴포넌트로 coolisionComp 지정
	//외관 크기 설정
	//충돌 비활성화
	compMeshWeaponBullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponBulletMeshComp"));
	compMeshWeaponBullet->SetupAttachment(compCollision);
	compMeshWeaponBullet->SetRelativeScale3D(FVector(0.25f)); //★★★추후 조정 필요. 블루프린트에서 조정 시 삭제
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
	compMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	compMovement->SetUpdatedComponent(compCollision);
	


	//[총알 발사체 컴포넌트 초기값 설정] //★★★전체 필요 없을 시 삭제
	//초기속도. InitialSpeed 속성 이용
	//최대속도. MaxSpeed 속성 이용
	//반동여부. bShouldBounce 속성 이용. true 할당
	//반동 값. 반동이 있다면 탄성은 어느 정도 될지 Bounciness 속성 이용
	//생명 시간 주기
	compMovement->InitialSpeed = 1000; //★★★추후 조정 필요
	compMovement->MaxSpeed = 1000; //★★★추후 조정 필요
	compMovement->bShouldBounce = true;
	compMovement->Bounciness = 0.8f; //★★★추후 조정 필요
	//InitialLifeSpan = 10.0f; //★★★추후 조정 필요
}

// Called when the game starts or when spawned
void ARIM_Bullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARIM_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

