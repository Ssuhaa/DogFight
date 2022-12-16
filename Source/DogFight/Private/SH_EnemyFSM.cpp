// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_EnemyFSM.h"
#include <SH_Player.h>
#include <SH_Enemy.h>
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
USH_EnemyFSM::USH_EnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USH_EnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()); //월드에서 타깃 찾아오기
	target = Cast<ASH_Player>(actor);
	me = Cast<ASH_Enemy>(GetOwner());

	// ...
	
}


// Called every frame
void USH_EnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	switch (mState) // 캐릭터 상태 변수에 따라 스위칭 시켜 해당 함수 호출하는 코드
	{
	case EEnemyState::Idle: 
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
	// ...
}


void USH_EnemyFSM::IdleState()//대기 상태 함수정의
{
	currentTime += GetWorld()->DeltaTimeSeconds; //시간흐름 저장.
	if (currentTime > idleDalayTime) //만약 경과시간이 대기시간을 초과했다면
	{
		mState = EEnemyState::Move; // 이동상태로 전환
		currentTime = 0; //경과시간 초기화
	}

}
void USH_EnemyFSM::MoveState()//이동 상태 함수정의
{
	FVector destination = target->GetActorLocation(); // 목적지를 타겟의 액터 로케이션으로 설정
	FVector dir = destination - me->GetActorLocation();
	me->AddMovementInput(dir.GetSafeNormal());
}
void USH_EnemyFSM::AttackState()//공격 상태 함수정의
{

}
void USH_EnemyFSM::DamageState() //피격 상태 함수정의
{

}
void USH_EnemyFSM::DieState() // 죽음 상태 함수정의
{

}