// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_EnemyFSM.h"
#include <SH_Player.h>
#include <SH_Enemy.h>
#include <Kismet/GameplayStatics.h>
#include <EnemyAnim.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetArrayLibrary.h>
#include <Containers/UnrealString.h>
#include "Weapon.h"

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
	me = Cast<ASH_Enemy>(GetOwner()); //소유 객체 가져오기
	// UEnemyAnim할당
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	SeachShortTarget();//가까운 적 찾기

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
	case EEnemyState::Pickup:
		PickupState();
		break;
	}
}

void USH_EnemyFSM::IdleState()//대기 상태 함수정의
{
	if (isDelay(idleDalayTime))
	{
		stateChange(EEnemyState::Move);
	}
}

void USH_EnemyFSM::MoveState()//이동 상태 함수정의
{
	FVector P;
	// 목적지를 타겟의 액터 로케이션으로 설정
	if (target != nullptr)
	{
		P = target->GetActorLocation() - me->GetActorLocation(); //타겟 방향
		if (target->GetName().Contains(TEXT("Player")) || target->GetName().Contains(TEXT("Enemy")))
		{
			if (P.Length() < attackRange) 
			{
				me->SetActorRotation(UKismetMathLibrary::MakeRotFromXZ(P, FVector::UpVector));// 타겟방향을 바라보게
				stateChange(EEnemyState::Attack);
			}
			else
			{
				me->AddMovementInput(P.GetSafeNormal());
			}
		}
		else if (target->GetName().Contains(TEXT("Weapon")))
		{
			if (P.Length() < 120.0)
			{
				SeachLongTarget();
			}
			else
			{
				me->AddMovementInput(P.GetSafeNormal()); 
			}
		}
	}
	else
	{
		SeachShortTarget();
	}


}


void USH_EnemyFSM::PickupState()
{

	if (isDelay(0.2))
	{
		SeachShortTarget();
		stateChange(EEnemyState::Idle);
	}

}

void USH_EnemyFSM::AttackState()//공격 상태 함수정의
{
	if (isDelay(attackDelayTime))
	{
		if (anim->isLollipopget || anim->isGunget)
		{
			anim->bAttackPlay = false;
		}
		else
		{
			anim->bAttackPlay = true;
		}

		float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation()); //타깃과의 거리 변수 담기
		if (distance > attackRange)
		{
			stateChange(EEnemyState::Move);
		}
	}

}

void USH_EnemyFSM::DamageState() //피격 상태 함수정의
{
	if (isDelay(damageDelayTime))
	{
		SeachShortTarget();
		stateChange(EEnemyState::Attack);
	}
}

void USH_EnemyFSM::DownState() //넉백 상태 함수 정의
{
	if (isDelay(dieDelayTime))
	{
		SeachLongTarget();
		stateChange(EEnemyState::Idle);
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
		hp--;
		randindex = FMath::RandRange(0, 1); //몽타주 인덱스 뽑기
		stateChangeMontage(EEnemyState::Damage, TEXT("Damage"));
	}
	else if (hp < 1 && downCount > 0)
	{
		stateChangeMontage(EEnemyState::Down, TEXT("Down"));

	}
	else
	{
		stateChangeMontage(EEnemyState::Die, TEXT("Die"));
	}

}

void USH_EnemyFSM::SeachShortTarget() //가까운 타겟 찾기
{
	addarray();
	for (int32 i = 0; i < targets.Num(); i++)
	{
		if (targets[i]->GetName().Contains(TEXT("Weapon")) || targets[i]->GetName().Contains(TEXT("Player")) || targets[i]->GetName().Contains(TEXT("Enemy")))
		{
			if (targets[i] != me)
			{
				float distance = FVector::Distance(targets[i]->GetActorLocation(), me->GetActorLocation());
				if (distance < dir)
				{
					dir = distance;
					target = Cast<ACharacter>(targets[i]);
					if (target == nullptr)
					{
						target = Cast<AWeapon>(targets[i]);
					}

				}
			}
		}
	}
}

void USH_EnemyFSM::SeachLongTarget() // 먼 타겟 찾기
{
	addarray();
	for (int32 i = 0; i < targets.Num(); i++)
	{
		if (targets[i]->GetName().Contains(TEXT("Weapon")) || targets[i]->GetName().Contains(TEXT("RIM")) || targets[i]->GetName().Contains(TEXT("SH")))
		{
			if (targets[i] != me)
			{
				float distance = FVector::Distance(targets[i]->GetActorLocation(), me->GetActorLocation());
				if (distance > dir)
				{
					dir = distance;
					target = Cast<ACharacter>(targets[i]);
					if (target == nullptr)
					{
						target = Cast<AWeapon>(targets[i]);
					}

				}
			}
		}
	}
}

void USH_EnemyFSM::stateChange(EEnemyState state)//스테이트 변경 후 애니메이션 동기화
{
	mState = state;
	anim->animState = mState;
	switch (state)
	{
	case EEnemyState::Idle:
		isAttackState = false;
		break;
	case EEnemyState::Move:
		isAttackState = false;
		break;
	case EEnemyState::Attack:
		currentTime = attackDelayTime;
		isAttackState = true;
		break;
	case EEnemyState::Damage:
		isAttackState = false;
		anim->Montage_Stop(damageDelayTime);
		break;
	case EEnemyState::Die:
		isAttackState = false;
		break;
	case EEnemyState::Down:
		isAttackState = false;
		anim->Montage_Stop(damageDelayTime);
		break;
	case EEnemyState::Pickup:
		isAttackState = false;
		break;
	default:
		break;
	}
}

void USH_EnemyFSM::stateChangeMontage(EEnemyState State, FString Name) //스테이트 변경 후 애님몽타주 플레이.
{

	mState = State;
	FString sectionName = FString::Printf(TEXT("%s%d"), *Name, randindex);
	anim->PlayDamagaAnim(FName(*sectionName));
	anim->animState = mState;

}

void USH_EnemyFSM::addarray() //캐릭터와 웨폰 어레이 수집
{
	targets.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), targets);
	if (anim->isGunget == false && anim->isLollipopget == false)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeapon::StaticClass(), Weaponarray);
		targets.Append(Weaponarray);
	}
}

bool USH_EnemyFSM::isDelay(float delaytime)
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > delaytime)
	{
		currentTime = 0;
		return true;
	}
	else
	{
		return false;
	}
}