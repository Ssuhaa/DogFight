// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_EnemyFSM.h"
#include <SH_Player.h>
#include <SH_Enemy.h>
#include <Kismet/GameplayStatics.h>
#include <EnemyAnim.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetArrayLibrary.h>
#include <Containers/UnrealString.h>

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
	SeachShortTarget();//가까운 적 찾기

	// UEnemyAnim할당
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

}

// Called every frame
void USH_EnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	currentTime += DeltaTime;
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
	if (currentTime > idleDalayTime)
	{
		isAttackState = false;
		stateChange(EEnemyState::Move);
		currentTime = 0; 
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
		stateChange(EEnemyState::Attack);
		anim->bAttackPlay = true;
		currentTime = attackDelayTime;
	}
	
}

void USH_EnemyFSM::AttackState()//공격 상태 함수정의
{
	if (currentTime > attackDelayTime)
	{
		isAttackState = true;
		float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation()); //타깃과의 거리 변수 담기
		if (distance > attackRange)
		{
			isAttackState = false;
			stateChange(EEnemyState::Move);
			currentTime = 0;
		}
		else
		{
			anim->bAttackPlay = true;
			stateChange(EEnemyState::Idle);
			currentTime = 0;

		}
	}

}

void USH_EnemyFSM::DamageState() //피격 상태 함수정의
{
	if (currentTime > damageDelayTime)
	{
		anim->Montage_Stop(damageDelayTime);
		SeachShortTarget();
		stateChange(EEnemyState::Attack);
		currentTime = 0;
	}
}

void USH_EnemyFSM::DownState() //넉백 상태 함수 정의
{
	if (currentTime > dieDelayTime)
	{
		anim->Montage_Stop(damageDelayTime);
		SeachLongTarget();
		stateChange(EEnemyState::Idle);
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
	
	if (hp > 0 && downCount > 0) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy HP : %d"), hp);
		randindex = FMath::RandRange(0, 1);
		stateChangeMontage(EEnemyState::Damage, TEXT("Damage"));
		currentTime = 0;
		hp--;
	}
	else if (hp < 1 && downCount > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Down : %d"), downCount);
		stateChangeMontage(EEnemyState::Down, TEXT("Down"));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy DIE"));
		stateChangeMontage(EEnemyState::Die, TEXT("Die"));
	}

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
}//가장 가까운타겟 찾기

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

void USH_EnemyFSM::stateChange(EEnemyState state)//스테이트 변경 후 애니메이션 동기화
{
	mState = state;
	anim->animState = mState;
} 

void USH_EnemyFSM::stateChangeMontage(EEnemyState State, FString Name) //스테이트 변경 후 애님몽타주 플레이.
{
	
	mState = State;
	FString sectionName = FString::Printf(TEXT("%s%d"), *Name, randindex);
	anim->PlayDamagaAnim(FName(*sectionName));
	anim->animState = mState;

} 