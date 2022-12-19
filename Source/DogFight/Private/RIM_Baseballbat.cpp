// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Baseballbat.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/SphereComponent.h>

// Sets default values
ARIM_Baseballbat::ARIM_Baseballbat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	//[야구방망이 스켈레탈메시 컴포넌트 등록]
	//야구방망이 스켈레탈메시 컴포넌트 등록
	//부모 컴포넌트를 Mesh 컴포넌트로 설정
	//스켈레탈메시 데이터 로드
	baseballbatMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BaseballbatComp"));
	baseballbatMeshComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempBaseballbatMesh(TEXT("PhysicsAsset'/Game/Animation/Charecter/Mesh/CharactorPhysicsAsets.CharactorPhysicsAsets'")); //★★★임시로 고양이 넣음

	//[야구방망이 스켈레탈메시 컴포넌트 데이터 설정]
	//스켈레탈메시 데이터 로드가 성공했다면
	//스켈레탈메시 데이터 할당
	//위치 조정하기

	if (TempBaseballbatMesh.Succeeded())
	{
		baseballbatMeshComp->SetSkeletalMesh(TempBaseballbatMesh.Object);
		baseballbatMeshComp->SetRelativeLocation(FVector(-14, 52, 120)); //★★★임시로 세팅. 추후 변경 필요
	}

	//[충돌체 컴포넌트 추가] ★★★현재 총알과 동일한 값으로 세팅 됨. 추후 변경 필요
	//충돌체 등록하기. 충돌체 인스턴스 등록하고 collisionComp 변수에 담기
	//충돌프로파일 설정. 충돌체의 충돌 프로파일을 BlockAll로 지정. 모든 물체와 부딪혀 튕귈 수 있게 하기
	//충돌체 크기 설정
	//충돌체를 루트 컴포넌트로 등록

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	collisionComp->SetSphereRadius(13);
	RootComponent = collisionComp;
	*/
}

// Called when the game starts or when spawned
void ARIM_Baseballbat::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARIM_Baseballbat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

