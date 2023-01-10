// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_EnemyFSM.h"
#include <SH_Player.h>
#include <SH_Enemy.h>
#include <Kismet/GameplayStatics.h>
#include <EnemyAnim.h>
#include <Kismet/KismetMathLibrary.h>
#include "Weapon.h"
#include "GunWeapon.h"
#include "LollipopWeapon.h"
#include <AIModule/Classes/AIController.h>

// Sets default values for this component's properties
USH_EnemyFSM::USH_EnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder <AGunWeapon> tempgun(TEXT("Class'/Script/DogFight.GunWeapon'"));
	if (tempgun.Succeeded())
	{
		Gun = tempgun.Class;
	}
	ConstructorHelpers::FClassFinder <ALollipopWeapon> tempLolli(TEXT("Class'/Script/DogFight.LollipopWeapon'"));
	if (tempLolli.Succeeded())
	{
		Lollipop = tempLolli.Class;
	}
	// ...
}


// Called when the game starts
void USH_EnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	me = Cast<ASH_Enemy>(GetOwner()); //소유 객체 가져오기
	// UEnemyAnim할당
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	addarray();
	RandomTarget();
	AI = Cast<AAIController>(me->GetController());
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
	case EEnemyState::Wakeup:
		WakeupState();
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

	// 목적지를 타겟의 액터 로케이션으로 설정
	if (target != nullptr)
	{
		P = target->GetActorLocation() - me->GetActorLocation(); //타겟 방향
		me->SetActorRotation(UKismetMathLibrary::MakeRotFromXZ(P, FVector::UpVector));// 타겟방향을 바라보게
		EPathFollowingRequestResult::Type Aireuslt = AI->MoveToLocation(target->GetActorLocation());
		if (target->GetName().Contains(TEXT("Player")) || target->GetName().Contains(TEXT("Enemy")))
		{
// 			if (Aireuslt == EPathFollowingRequestResult::AlreadyAtGoal)
// 			{
// 				stateChange(EEnemyState::Attack);
// 
// 			}
// 
			if (P.Length() < attackRange)
			{
				 stateChange(EEnemyState::Attack);
			}
// 			else
// 			{
// 				me->AddMovementInput(P.GetSafeNormal());
// 			}
		}
		else if (target->GetName().Contains(TEXT("Weapon")))
		{
			/*EPathFollowingRequestResult::Type Aireuslt = AI->MoveToLocation(target->GetActorLocation());*/
// 			if (Aireuslt == EPathFollowingRequestResult::AlreadyAtGoal)
// 			{
// 				RandomTarget();
// 
// 			}
// 
			if (P.Length() < 120.0)
			{
				RandomTarget();
			}
// 			else
// 			{
// 				me->AddMovementInput(P.GetSafeNormal());
// 			}


		}
	}
	else
	{
		RandomTarget();
	}

}


void USH_EnemyFSM::PickupState()
{

	if (isDelay(PickupDelayTime))
	{
		stateChange(EEnemyState::Idle);
	}

}

void USH_EnemyFSM::AttackState()//공격 상태 함수정의
{
	P = target->GetActorLocation() - me->GetActorLocation();
	me->SetActorRotation(UKismetMathLibrary::MakeRotFromXZ(P, FVector::UpVector));
	if (isDelay(attackDelayTime))
	{

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
		stateChange(EEnemyState::Idle);
	}
}

void USH_EnemyFSM::DownState() //넉백 상태 함수 정의
{
	if (isDelay(downDelayTime))
	{
		stateChangeMontage(EEnemyState::Wakeup, TEXT("Wakeup"));
		downCount--;
		hp = 5;
	}
}

void  USH_EnemyFSM::WakeupState()
{
	if (isDelay(WakeupDelayTime))
	{
		stateChange(EEnemyState::Idle);
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


void USH_EnemyFSM::addarray() //캐릭터와 웨폰 어레이 수집
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), targets);
	addWeaponArray();
}

void USH_EnemyFSM::removeWeaponArray()
{
	for (int32 i = 0; i < Weaponarray.Num(); i++)
	{
		targets.Remove(Weaponarray[i]);
	}

}
void USH_EnemyFSM::addWeaponArray()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeapon::StaticClass(), Weaponarray);
	targets.Append(Weaponarray);
}

void USH_EnemyFSM::RandomTarget() //랜덤 타겟 찾기
{
	int32 randTargetindex = FMath::RandRange(0, targets.Num() - 1);
	target = targets[randTargetindex];
	if (target != me)
	{
		ASH_Enemy* currentTarget = Cast <ASH_Enemy>(target);
		if (currentTarget != nullptr)
		{
			if (currentTarget->fsm->mState == EEnemyState::Die || currentTarget->fsm->mState == EEnemyState::Down)
			{
				RandomTarget();
			}
		}
	}
	else
	{
		RandomTarget();
	}


	me->SetActorRotation(FRotator::ZeroRotator);
}


void USH_EnemyFSM::stateChange(EEnemyState state)//스테이트 변경 후 초기설정 및 애니메이션 동기화.
{
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s-------->%s"), *enumPtr->GetNameStringByIndex((int32)mState), *enumPtr->GetNameStringByIndex((int32)state));
	}
	AI->StopMovement();
	mState = state;
	anim->animState = mState;
	switch (state)
	{
	case EEnemyState::Idle:
		anim->Montage_Stop(damageDelayTime);
		RandomTarget();
		break;
	case EEnemyState::Move:
		if (anim->isGunget)
		{
			anim->bAttackPlay = false;
			attackRange = GunRange;
		}
		else if (anim->isLollipopget)
		{
			anim->bAttackPlay = false;
			attackRange = LollipopRange;
		}
		else
		{
			anim->bAttackPlay = true;
			attackRange = defaultRange;
		}
		break;
	case EEnemyState::Attack:
		currentTime = attackDelayTime;
		if (anim->isGunget)
		{
			anim->bAttackPlay = false;
			attackRange = GunRange;
		}
		else if (anim->isLollipopget)
		{
			anim->bAttackPlay = false;
			attackRange = LollipopRange;
		}
		else
		{
			anim->bAttackPlay = true;
			attackRange = defaultRange;
		}
		break;
	case EEnemyState::Pickup:

		break;
	}
}

void USH_EnemyFSM::stateChangeMontage(EEnemyState State, FString Name) //스테이트 변경 후 애님몽타주 플레이.
{
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s-------->%s"), *enumPtr->GetNameStringByIndex((int32)mState), *enumPtr->GetNameStringByIndex((int32)State));
	}
	AI->StopMovement();
	mState = State;
	anim->animState = mState;
	FString sectionName = FString::Printf(TEXT("%s%d"), *Name, randindex);
	anim->PlayDamagaAnim(FName(*sectionName));

	switch (State)
	{
	case EEnemyState::Damage:
		RandomTarget();
		DropWeapon();
		break;
	case EEnemyState::Die:
		removeDieTarget();
		break;
	case EEnemyState::Down:
		DropWeapon();
		break;
	}

}

bool USH_EnemyFSM::isDelay(float delaytime) // 딜레이 함수
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

void USH_EnemyFSM::DropWeapon() //무기 해제
{
	if (anim->isGunget == true)
	{
		GetWorld()->SpawnActor<AGunWeapon>(Gun, me->GetActorLocation() + FVector(0, 50, 50), me->GetActorRotation());
		anim->isGunget = false;

	}
	else if (anim->isLollipopget == true)
	{
		GetWorld()->SpawnActor<ALollipopWeapon>(Lollipop, me->GetActorLocation() + FVector(0, 50, 50), me->GetActorRotation());
		anim->isLollipopget = false;
	}
	me->compMesh->SetStaticMesh(nullptr);
	addWeaponArray();
}

void USH_EnemyFSM::removeDieTarget() // 죽은 타겟 지우기
{
	for (int32 i = 0; i < targets.Num(); i++)
	{
		ASH_Enemy* currentenemy = Cast<ASH_Enemy>(targets[i]);
		if (currentenemy != nullptr)
		{
			currentenemy->fsm->targets.Remove(me);
			currentenemy->fsm->RandomTarget();

		}
	}
}