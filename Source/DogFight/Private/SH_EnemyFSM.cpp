// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_EnemyFSM.h"
#include <SH_Enemy.h>
#include <Kismet/GameplayStatics.h>
#include <EnemyAnim.h>
#include <Kismet/KismetMathLibrary.h>
#include "Weapon.h"
#include "GunWeapon.h"
#include "LollipopWeapon.h"
#include <AIModule/Classes/AIController.h>
#include "RIM_Player.h"
#include "ItemSpawn.h"
#include "../DogFightGameModeBase.h"
#include "DownWidget.h"
#include "Timer.h"
#include <Components/VerticalBox.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <NavigationSystem.h>

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
	ConstructorHelpers::FClassFinder<UDownWidget> tempDownWG(TEXT("WidgetBlueprint'/Game/BluePrint/BP_down.BP_down_C'"));
	if (tempDownWG.Succeeded())
	{
		downUI = tempDownWG.Class;
	}
	ConstructorHelpers::FClassFinder<UDownWidget> tempDieWG(TEXT("WidgetBlueprint'/Game/BluePrint/BP_die.BP_die_C'"));
	if (tempDieWG.Succeeded())
	{
		dieUI = tempDieWG.Class;
	}
}


// Called when the game starts
void USH_EnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	me = Cast<ASH_Enemy>(GetOwner()); //소유 객체 가져오기
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
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
	case EEnemyState::Run:
		RunState();
		break;
	case EEnemyState::Jump:
		JumpState();
		break;
	}

	if (me->GetVelocity().Length() > 650) //벨로시티가 650이상이면 강제로 600으로 맞춰준다.
	{
		FVector v = me->GetVelocity().GetSafeNormal();
		me->GetCharacterMovement()->Velocity = v * 600;
	}
}

void USH_EnemyFSM::IdleState()//대기 상태 함수
{
	if (isDelay(idleDalayTime))
	{
		stateChange(EEnemyState::Move);
	}
}

void USH_EnemyFSM::MoveState()//이동 상태 함수
{
	if (target != NULL)
	{
		EPathFollowingRequestResult::Type Aireuslt = AI->MoveToLocation(target->GetActorLocation());
		P = target->GetActorLocation() - me->GetActorLocation(); //타겟 방향
		me->SetActorRotation(UKismetMathLibrary::MakeRotFromXZ(P, FVector::UpVector));// 타겟방향을 바라보게
		if (target->GetName().Contains(TEXT("Player")) || target->GetName().Contains(TEXT("Enemy")))
		{
			ASH_Enemy* currTE = Cast<ASH_Enemy>(target);
			if (currTE != nullptr)
			{
				if (currTE->fsm->mState == EEnemyState::Die || currTE->fsm->mState == EEnemyState::Down)
				{
					stateChange(EEnemyState::Idle);
				}
			}
			if (P.Length() < attackRange)
			{
				stateChange(EEnemyState::Attack);
			}
			else if (me->GetActorLocation().Z < 0 &&  target->GetActorLocation().Z > 0)
			{
				if (me->GetCharacterMovement()->IsFalling() == false)
				{	
					stateChange(EEnemyState::Jump);
					me->Jump();

				}
			}
		}
		else if (target->GetName().Contains(TEXT("Weapon")))
		{
			if (P.Length() < 120.0)
			{
				RandomTarget();
			}
			else if (me->GetActorLocation().Z < 0 && target->GetActorLocation().Z > -80)
			{	
				if (me->GetCharacterMovement()->IsFalling() == false)
				{
					stateChange(EEnemyState::Jump);
					me->Jump();
				}
			}
		}
		
	}
	else
	{
		targets.Remove(target);
		RandomTarget();
	}

}
void USH_EnemyFSM::JumpState()
{
	if (me->GetCharacterMovement()->IsFalling() == false)
	{
		stateChange(EEnemyState::Move);
	}
	else
	{	
		me->SetActorLocation(me->GetActorLocation() + me->GetActorForwardVector() * GetWorld()->DeltaTimeSeconds * 500);
	}
}

void USH_EnemyFSM::PickupState() // 줍기 상태 함수
{
	if (isDelay(PickupDelayTime))
	{
		stateChange(EEnemyState::Idle);
	}
}

void USH_EnemyFSM::AttackState()//공격 상태 함수
{
	P = target->GetActorLocation() - me->GetActorLocation();
	me->SetActorRotation(UKismetMathLibrary::MakeRotFromXZ(P, FVector::UpVector));
	if (isDelay(attackDelayTime))
	{
		float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation()); //타깃과의 거리 변수 담기
		if (distance > attackRange)
		{
			stateChange(EEnemyState::Idle);
		}
	}
}

void USH_EnemyFSM::DamageState() //피격 상태 함수정의
{
	if (isDelay(damageDelayTime))
	{
		stateChange(EEnemyState::Run);
	}
}
void  USH_EnemyFSM::RunState()
{
	EPathFollowingRequestResult::Type Aireuslt = AI->MoveToLocation(randPos);
	if (Aireuslt == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		stateChange(EEnemyState::Idle);
	}
	if(isDelay(RunDelayTime))
	{
		stateChange(EEnemyState::Idle);
	}
}

void USH_EnemyFSM::DownState() //넉백 상태 함수 정의
{
	if (isDelay(downDelayTime))
	{
		stateChangeMontage(EEnemyState::Wakeup, TEXT("Wakeup"));
		hp = 3;
	}
}

void  USH_EnemyFSM::WakeupState() // 일어나는 상태 함수
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
	else if (hp <= 0 && downCount > 0)
	{
		downCount--;
		currDown++;
		stateChangeMontage(EEnemyState::Down, TEXT("Down"));
	}
	else 
	{
		stateChangeMontage(EEnemyState::Die, TEXT("Die"));
	}

}


void USH_EnemyFSM::RandomTarget() //랜덤 타겟 찾기
{
	int32 randTargetindex = FMath::RandRange(0, targets.Num() - 1);
	target = targets[randTargetindex];

	ASH_Enemy* currentTarget = Cast <ASH_Enemy>(target);
	if (currentTarget != nullptr)
	{
		if (currentTarget->fsm->mState == EEnemyState::Die || currentTarget->fsm->mState == EEnemyState::Down)
		{
			RandomTarget();
		}
	
	}
	AWeapon* CurrentWeaponTarget = Cast<AWeapon>(target);
	if (CurrentWeaponTarget != nullptr)
	{
		if (anim->isGunget || anim->isLollipopget||anim->isTennis||anim->isShovel)
		{
			RandomTarget();
		}
	}

}


void USH_EnemyFSM::stateChange(EEnemyState state)//스테이트 변경 후 초기설정 및 애니메이션 동기화.
{
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s-------->%s"), *enumPtr->GetNameStringByIndex((int32)mState), *enumPtr->GetNameStringByIndex((int32)state));
	}
	AI->StopMovement();
	anim->Montage_Stop(damageDelayTime);
	mState = state;
	anim->animState = mState;
	switch (state)
	{
	case EEnemyState::Idle:
		RandomTarget();
		me->SetActorRotation(FRotator::ZeroRotator);
		break;
	case EEnemyState::Move:
		if (anim->isGunget)
		{
			WeaponAnimChange(false, GunRange);

		}
		else if (anim->isLollipopget)
		{
			WeaponAnimChange(false, LollipopRange);
		}
		else
		{
			WeaponAnimChange(true, defaultRange);
		}
		break;
	case EEnemyState::Attack:
		currentTime = attackDelayTime;
		if (anim->isGunget)
		{
			WeaponAnimChange(false, GunRange);

		}
		else if (anim->isLollipopget)
		{
			WeaponAnimChange(false, LollipopRange);
		}
		else if(anim->isShovel)
		{
			WeaponAnimChange(false, ShovelRange);
		}
		else if(anim->isTennis)
		{
			WeaponAnimChange(false, TennisRange);
		}
		else
		{
			WeaponAnimChange(true, defaultRange);
		}
		break;
	case EEnemyState::Pickup:
		
		break;
	case EEnemyState::Run:
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(me->GetActorLocation(), 1000, loc);
		randPos = loc.Location;
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

	mState = State;
	anim->animState = mState;
	FString sectionName = FString::Printf(TEXT("%s%d"), *Name, randindex);
	anim->PlayDamagaAnim(FName(*sectionName));

	AI->StopMovement();

	switch (State)
	{
	case EEnemyState::Damage:
		DropWeapon();
		break;
	case EEnemyState::Die:
		DropWeapon();
		removeDieTarget();
		SstDieUl();
		break;
	case EEnemyState::Down:
		SetDownUI();
		DropWeapon();
		break;
	case EEnemyState::Wakeup:
		
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
	AItemSpawn* ItemSpawn = Cast<AItemSpawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AItemSpawn::StaticClass()));
	if (anim->isGunget == true)
	{
		ItemSpawn->CreateWeapon(0, me->GetActorLocation() + FVector(0, 50, 50), me->GetActorRotation());
		anim->isGunget = false;

	}
	else if (anim->isLollipopget == true)
	{
		ItemSpawn->CreateWeapon(1, me->GetActorLocation() + FVector(0, 50, 50), me->GetActorRotation());
		anim->isLollipopget = false;
	}
	else if(anim->isShovel == true)
	{
		ItemSpawn->CreateWeapon(2, me->GetActorLocation() + FVector(0, 50, 50), me->GetActorRotation());
		anim->isShovel = false;
	}
	else if (anim->isTennis == true)
	{
		ItemSpawn->CreateWeapon(3, me->GetActorLocation() + FVector(0, 50, 50), me->GetActorRotation());
		anim->isTennis = false;
	}
	me->compMesh->SetStaticMesh(nullptr);
}

void USH_EnemyFSM::removeDieTarget() // 죽은 타겟 지우기
{
	dieDelegate.Broadcast(me); //내가 죽으면 딜리게이트의 바인딩된 removeTarget 함수에 나를 전달한다.
}

void USH_EnemyFSM::TargetDotAttack()
{
	for (int32 i = 0; i < targets.Num(); i++)
	{
		enemy = Cast<ASH_Enemy>(targets[i]);
		if (enemy != nullptr)
		{
			if (isTargetinAttackRange(targets[i]))
			{
				if (enemy->fsm->mState != EEnemyState::Down && enemy->fsm->mState != EEnemyState::Die && enemy->fsm->mState != EEnemyState::Damage)
				{
					enemy->fsm->OnDamageProcess();
				}
				else
				{
					RandomTarget();
					stateChange(EEnemyState::Idle);
				}

			}
		}
		player = Cast<ARIM_Player>(targets[i]);
		if (player != nullptr)
		{
			if (isTargetinAttackRange(targets[i]))
			{
				//if(player->isplayerDown == true || player->isplayerDie == true)
				if(player->playerState == EPlayerState::KnockDown || player->playerState == EPlayerState::Die)
				{
					RandomTarget();
					stateChange(EEnemyState::Idle);
				}
				else
				{
					
					player->OnDamageProcess();
				}
			}
		}
	}

}


bool USH_EnemyFSM::isTargetinAttackRange(AActor* Target)
{
	FVector dir = Target->GetActorLocation() - me->GetActorLocation();
	float dotValue = FVector::DotProduct(me->GetActorForwardVector(), dir.GetSafeNormal());
	float angle = UKismetMathLibrary::DegAcos(dotValue);
	if (angle < EnemyAngle && dir.Length() < traceRange)
	{
		return true;
	}
	return false;
}


void  USH_EnemyFSM::WeaponAnimChange(bool AttackPlay, float Range) //웨폰
{
	anim->bAttackPlay = AttackPlay;
	attackRange = Range;
}

void USH_EnemyFSM::SetDownUI()
{
	class UDownWidget* DownWG = CreateWidget<UDownWidget>(GetWorld(), downUI);
	DownWG->Name = me->GetName();
	DownWG->DownCount = currDown;
	
	ADogFightGameModeBase* gamemode = GetWorld()->GetAuthGameMode<ADogFightGameModeBase>();
	gamemode->TimeWG->VerticalDown->AddChildToVerticalBox(DownWG);
}

void USH_EnemyFSM::SstDieUl()
{
	class UDownWidget* DieWG = CreateWidget<UDownWidget>(GetWorld(), dieUI);
	DieWG->Name = me->GetName();

	ADogFightGameModeBase* gamemode = GetWorld()->GetAuthGameMode<ADogFightGameModeBase>();
	gamemode->TimeWG->VerticalDown->AddChildToVerticalBox(DieWG);
}