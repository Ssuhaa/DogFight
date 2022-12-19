// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Gun.h"
#include <Components/SkeletalMeshComponent.h>

// Sets default values
ARIM_Gun::ARIM_Gun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	//[총 스켈레탈메시 컴포넌트 등록]
	//총 스켈레탈메시 컴포넌트 등록
	//부모 컴포넌트를 Mesh 컴포넌트로 설정
	//스켈레탈메시 데이터 로드

	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor1.Charactor1'")); //★★★임시로 토기 넣음

	//[총 스켈레탈메시 컴포넌트 데이터 설정]
	//스켈레탈메시 데이터 로드가 성공했다면
	//스켈레탈메시 데이터 할당
	//위치 조정하기

	if (TempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(0, 0, 50)); //★★★임시로 세팅. 추후 변경 필요
	}
	*/
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



