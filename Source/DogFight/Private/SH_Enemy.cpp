// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Enemy.h"
#include "SH_EnemyFSM.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/Character.h>
#include <Components/BoxComponent.h>
#include "SH_Player.h"


// Sets default values
ASH_Enemy::ASH_Enemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor1.Charactor1'")); //메시 데이터 찾기
	if (TempMesh.Succeeded())//만약 메쉬데이터를 찾았다면
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object); // 에너미 메쉬데이터 설정
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0)); // 에너미 위치와 회전값 설정
	}

	fsm = CreateDefaultSubobject<USH_EnemyFSM>(TEXT("FSM")); //	에너미에 FSM컴포넌트 추가


	GetCharacterMovement()->bOrientRotationToMovement = true;
	// 애니메이션 블루프린트 할당
	ConstructorHelpers::FClassFinder<UAnimInstance> tempclass(TEXT("AnimBlueprint'/Game/BluePrint/ABP_EnemyAnim.ABP_EnemyAnim_C'")); //끝에 C를 써줘야 오류가 나지않음
	if (tempclass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempclass.Class);
	}
	//어택 콜리전
	compAttack = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	compAttack->SetupAttachment(GetMesh(), TEXT("Prop_Socket"));
	compAttack->SetBoxExtent(FVector(25));

	//!!!!!!!!에너미 콜리전 수정할것.

}

// Called when the game starts or when spawned
void ASH_Enemy::BeginPlay()
{
	Super::BeginPlay();
	compAttack->OnComponentBeginOverlap.AddDynamic(this, &ASH_Enemy::attackBoxBeginOverlap); //컴포넌트 비긴오버랩 델리게이트 바인딩
	compAttack->OnComponentEndOverlap.AddDynamic(this, &ASH_Enemy::attackBoxEndOverlap);
}

// Called every frame
void ASH_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void ASH_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ASH_Enemy::attackBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != fsm->me)
	{
		if (OtherActor->GetName().Contains(TEXT("Enemy")))
		{
			currEnemy = Cast<ASH_Enemy>(OtherActor);
			if (fsm->isAttackState == true)
			{
				if (fsm->mState == EEnemyState::Down || fsm->mState == EEnemyState::Die || fsm->mState == EEnemyState::Damage) return;
				if (currEnemy->fsm->mState != EEnemyState::Down && currEnemy->fsm->mState != EEnemyState::Die && currEnemy->fsm->mState != EEnemyState::Damage)
				{
					currEnemy->fsm->OnDamageProcess();
				}
				else
				{
					fsm->SeachLongTarget();
				}


			}

		}
		else if (OtherActor->GetName().Contains(TEXT("Player")))
		{
			//!!!!!!!!if 플레이어가 맞았는지 유무 판단. 멀쩡한 상태면
			//!!!!!!!!시간 지난 후 데미지 들어가는게 필요함.
			player = Cast<ASH_Player>(OtherActor);
			player->OnDamageProcess();
		}
	}
}


void ASH_Enemy::attackBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}