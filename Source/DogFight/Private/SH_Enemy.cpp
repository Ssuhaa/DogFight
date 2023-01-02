// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Enemy.h"
#include "SH_EnemyFSM.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/Character.h>
#include <Components/BoxComponent.h>
#include "RIM_Player.h"
#include "SH_Player.h"
#include "EnemyAnim.h"


// Sets default values
ASH_Enemy::ASH_Enemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SkeletalMesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor1.Charactor1'")); 
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object); 
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0)); 
	}
	//Fsm
	fsm = CreateDefaultSubobject<USH_EnemyFSM>(TEXT("FSM"));

	//Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// AnimInstance
	ConstructorHelpers::FClassFinder<UAnimInstance> tempclass(TEXT("AnimBlueprint'/Game/BluePrint/ABP_EnemyAnim.ABP_EnemyAnim_C'")); //끝에 C를 써줘야 오류가 나지않음
	if (tempclass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempclass.Class);
	}
	//Weapon
	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	compMesh->SetupAttachment(GetMesh(), TEXT("Prop_RSocket"));


	//AttackCollision Hand
	compAttack = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	compAttack->SetupAttachment(GetMesh(), TEXT("Prop_RSocket"));
	compAttack->SetBoxExtent(FVector(25));
	//AttackCollision Foot
	compAttack2 = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionfoot"));
	compAttack2->SetupAttachment(GetMesh(), TEXT("Foot_Socket"));
	compAttack2->SetBoxExtent(FVector(25));

}

// Called when the game starts or when spawned
void ASH_Enemy::BeginPlay()
{
	Super::BeginPlay();
	compAttack->OnComponentBeginOverlap.AddDynamic(this, &ASH_Enemy::attackBoxBeginOverlap);
	compAttack2->OnComponentBeginOverlap.AddDynamic(this, &ASH_Enemy::attackBoxBeginOverlap);//컴포넌트 비긴오버랩 델리게이트 바인딩
	
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
			//if (fsm->isAttackState == true)
			if(fsm->target == currEnemy)
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

void ASH_Enemy::GetEnemyWeapon(UStaticMesh* WeaponMesh, FString soketname)
{
	compMesh->SetStaticMesh(WeaponMesh);
	compMesh->SetupAttachment(compMesh, FName(*soketname));
}
