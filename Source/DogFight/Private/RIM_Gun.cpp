// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Gun.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
ARIM_Gun::ARIM_Gun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[충돌체 컴포넌트 추가]
	//충돌체 등록하기. 충돌체 인스턴스 등록하고 collisionComp 변수에 담기
	//충돌프로파일 설정. 충돌체의 충돌 프로파일을 BlockAll로 지정. 모든 물체와 부딪혀 튕귈 수 있게 하기
	//충돌체 크기 설정
	//충돌체를 루트 컴포넌트로 등록

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	collisionComp->SetSphereRadius(13); //★★★추후 조정 필요. 무엇인지 모르겠다. 블루프린트에서 조정 시 삭제
	RootComponent = collisionComp;



	//[스테틱메시 컴포넌트 추가]
	//외관 컴포넌트 등록하기. UStaticMeshComponent 인스턴스 만들고 bodyMeshComp 멤버 변수에 할당
	//부모 컴포넌트 지정. 부모 컴포넌트로 coolisionComp 지정
	//충돌 비활성화
	//외관 크기 설정

	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gunMeshComp"));
	bodyMeshComp->SetupAttachment(collisionComp);
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bodyMeshComp->SetRelativeScale3D(FVector(0.25f)); //★★★추후 조정 필요. 블루프린트에서 조정 시 삭제



	//[총 컴포넌트 추가]
	//총 컴포넌트
	//movement 컴포넌트가 생산시킬 컴포넌트 지정

	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	movementComp->SetUpdatedComponent(collisionComp);



	//[총 컴포넌트 초깃값 설정]
	//초기속도. InitialSpeed 속성 이용
	//최대속도. MaxSpeed 속성 이용
	//반동여부. bShouldBounce 속성 이용. true 할당
	//반동 값. 반동이 있다면 탄성은 어느 정도 될지 Bounciness 속성 이용
	//생명 시간 주기

	movementComp->InitialSpeed = 500; //★★★추후 조정 필요
	movementComp->MaxSpeed = 500; //★★★추후 조정 필요
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.2f; //★★★추후 조정 필요
	InitialLifeSpan = 30.0f; //★★★추후 조정 필요


}

// Called when the game starts or when spawned
void ARIM_Gun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARIM_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}