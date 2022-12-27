// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_EnemyFSM.h"
#include <SH_Player.h>
#include <SH_Enemy.h>
#include <Kismet/GameplayStatics.h>
#include <EnemyAnim.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetArrayLibrary.h>

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

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), targets); //모든 액터 배열로 가져오기
	me = Cast<ASH_Enemy>(GetOwner()); //소유 객체 가져오기
	SeachShortTarget();

	// UEnemyAnim할당
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

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
	case  EEnemyState::Down:
		DownState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
}

void USH_EnemyFSM::IdleState()//대기 상태 함수정의
{
	currentTime += GetWorld()->DeltaTimeSeconds; //시간흐름 저장.
	if (currentTime > idleDalayTime) //만약 경과시간이 대기시간을 초과했다면
	{
		isAttackState = false;
		mState = EEnemyState::Move; // 이동상태로 전환
		anim->animState = mState;
		currentTime = 0; //경과시간 초기화
	}
}

void USH_EnemyFSM::MoveState()//이동 상태 함수정의
{
	// 목적지를 타겟의 액터 로케이션으로 설정
	FVector P = target->GetActorLocation() - me->GetActorLocation(); //타겟 방향
	me->AddMovementInput(P.GetSafeNormal()); //타겟 방향으로 이동
	me->SetActorRotation(UKismetMathLibrary::MakeRotFromXZ(P, FVector::UpVector));// 타겟방향을 바라보게
	if (P.Size() < attackRange) //만약 타깃과의 거리가 공격범위 안에 들어오면
	{
		mState = EEnemyState::Attack; // 공격상태로 전환
		anim->animState = mState;
		anim->bAttackPlay = true;
		currentTime = attackDelayTime;
	}
	
}

void USH_EnemyFSM::AttackState()//공격 상태 함수정의
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > attackDelayTime)
	{
		isAttackState = true;
		float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation()); //타깃과의 거리 변수 담기
		if (distance > attackRange) // 타깃과의 거리가 공격범위보다 벗어나면
		{
			isAttackState = false;
			mState = EEnemyState::Move; //에너미 상태 무브로 이동.
			anim->animState = mState;
			currentTime = 0;
		}
		anim->bAttackPlay = true;
		mState = EEnemyState::Idle;
		anim->animState = mState;
		currentTime = 0; //경과시간 초기화
	}

}

void USH_EnemyFSM::DamageState() //피격 상태 함수정의
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > damageDelayTime) //피격대기시간이 지나면 대기상태로 전환
	{
		anim->Montage_Stop(damageDelayTime);
		mState = EEnemyState::Attack;
		anim->animState = mState;
		currentTime = 0;

	}
}

void USH_EnemyFSM::DownState() //넉백 상태 함수 정의
{
	currentTime += GetWorld()->DeltaRealTimeSeconds;
	if (currentTime > dieDelayTime)
	{
		anim->Montage_Stop(damageDelayTime);
		SeachLongTarget();
		mState = EEnemyState::Idle; // 대기 스테이트 전환
		anim->animState = mState;
		currentTime = 0;
		downCount--;
		hp = 5;
	}
}

void USH_EnemyFSM::DieState() // 죽음 상태 함수 정의.
{


}

void  USH_EnemyFSM::OnDamageProcess() //피격알림 이벤트 함수 정의
{

	if (hp > 0 && downCount > 0) //체력이 0이아니고 다운카운트가 0이아니면 피격상태 유지
	{
		UE_LOG(LogTemp, Warning, TEXT("HP : %d"), hp);
		mState = EEnemyState::Damage;
		randindex = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), randindex);
		anim->PlayDamagaAnim(FName(*sectionName));
		currentTime = 0;
		hp--;
	}
	else if (hp < 1 && downCount > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("	Down : %d"), downCount);
		UE_LOG(LogTemp, Warning, TEXT("Enemy Fall down!"));
		mState = EEnemyState::Down;
		FString sectionName = FString::Printf(TEXT("Down%d"), randindex);
		anim->PlayDamagaAnim(FName(*sectionName));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy DIE"));
		mState = EEnemyState::Die;
		FString sectionName = FString::Printf(TEXT("Down%d"), randindex);
		anim->PlayDamagaAnim(FName(*sectionName));

	}
	anim->animState = mState;

}

void USH_EnemyFSM::SeachShortTarget() //가까운 타겟 찾기
{

	for (int32 i = 0; i < targets.Num(); i++)
	{
		if (targets[i] != me)
		{
			float distance = FVector::Distance(targets[i]->GetActorLocation(), me->GetActorLocation());
			if (distance < dir)
			{
				dir = distance;
				target = Cast<ACharacter>(targets[i]);

			}
		}
	}
}

void USH_EnemyFSM::SeachLongTarget() // 먼 타겟 찾기
{
	for (int32 i = 0; i < targets.Num(); i++)
	{
		if (targets[i] != me)
		{
			float distance = FVector::Distance(targets[i]->GetActorLocation(), me->GetActorLocation());
			if (distance > dir)
			{
				dir = distance;
				target = Cast<ACharacter>(targets[i]);

			}
		}
	}
}