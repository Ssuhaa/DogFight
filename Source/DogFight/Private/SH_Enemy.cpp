// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Enemy.h"
#include "SH_EnemyFSM.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/Character.h>
#include <Components/BoxComponent.h>
#include "RIM_Player.h"
#include "SH_Player.h"
#include "EnemyAnim.h"
#include <Components/CapsuleComponent.h>
#include <NavigationInvokerComponent.h>


// Sets default values
ASH_Enemy::ASH_Enemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SkeletalMesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh1(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Charactor1.Charactor1'")); 
	if (TempMesh1.Succeeded())
	{
		randmesh.Add(TempMesh1.Object);
	}
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh2(TEXT("SkeletalMesh'/Game/Animation/Charecter/Mesh/Chractor3.Chractor3'"));
	if (TempMesh2.Succeeded())
	{
		randmesh.Add(TempMesh2.Object);
	}

	int32 meshindex = FMath::RandRange(0, randmesh.Num()-1);
	GetMesh()->SetSkeletalMesh(randmesh[meshindex]); 
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0)); 
	GetMesh()->SetCollisionProfileName("Enemy");
	//Fsm
	fsm = CreateDefaultSubobject<USH_EnemyFSM>(TEXT("FSM"));

	//Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// AnimInstance
	ConstructorHelpers::FClassFinder<UAnimInstance> tempclass(TEXT("AnimBlueprint'/Game/BluePrint/ABP_EnemyAnim.ABP_EnemyAnim_C'")); 
	if (tempclass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempclass.Class);
	}
	//Weapon
	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	compMesh->SetupAttachment(GetMesh(), TEXT("Prop_RSocket"));
	compMesh->SetCollisionProfileName("Enemy");


	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Navi = CreateDefaultSubobject<UNavigationInvokerComponent> (TEXT("NavigationInvoker"));
	Navi->SetGenerationRadii(1000,1200);

}

// Called when the game starts or when spawned
void ASH_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
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


void ASH_Enemy::GetEnemyWeapon(UStaticMesh* WeaponMesh, FString soketname)
{
	compMesh->SetStaticMesh(WeaponMesh);
	compMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform ,FName(*soketname));
}
