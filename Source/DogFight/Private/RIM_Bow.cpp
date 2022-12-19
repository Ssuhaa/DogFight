// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Bow.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ARIM_Bow::ARIM_Bow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	//[활 스켈레탈메시 컴포넌트 등록]
	//활 스켈레탈메시 컴포넌트 등록
	//부모 컴포넌트를 Mesh 컴포넌트로 설정
	//스켈레탈메시 데이터 로드
	bowMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BowMeshComp"));
	bowMeshComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempBowMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor2.Charactor2'")); //★★★임시로 멍멍이 넣음

	//[활 스켈레탈메시 컴포넌트 데이터 설정]
	//스켈레탈메시 데이터 로드가 성공했다면
	//스켈레탈메시 데이터 할당
	//위치 조정하기

	if (TempBowMesh.Succeeded())
	{
		bowMeshComp->SetSkeletalMesh(TempBowMesh.Object);
		bowMeshComp->SetRelativeLocation(FVector(0, 50, 120)); //★★★임시로 세팅. 추후 변경 필요
	}
	*/
}

// Called when the game starts or when spawned
void ARIM_Bow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARIM_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

