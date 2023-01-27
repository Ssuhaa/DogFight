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
#include <AIModule/Classes/AIController.h>


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
	GetMesh()->SetupAttachment(RootComponent);

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
	compMesh->SetCollisionProfileName("NoCollision");
	


	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

// 	Navi = CreateDefaultSubobject<UNavigationInvokerComponent> (TEXT("NavigationInvoker"));
// 	Navi->SetGenerationRadii(1000,1500);

	PhysicComp = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnim"));

// 	DeadBlock = CreateDefaultSubobject<UStaticMeshComponent> (TEXT("DeadBlock"));
// 	ConstructorHelpers::FObjectFinder <UStaticMesh> tempCube(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
// 	if (tempCube.Succeeded())
// 	{
// 		DeadBlock->SetStaticMesh(tempCube.Object);
// 	}
// 	DeadBlock->SetupAttachment(GetMesh());
// 	DeadBlock->SetRelativeLocation( FVector(0, -30,170));
// 	DeadBlock->SetRelativeScale3D(FVector(0.75f, 1.5f, 2.0f));
// 	DeadBlock->SetCollisionResponseToAllChannels(ECR_Ignore);
// 	DeadBlock->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Block);
// 	DeadBlock->SetVisibility(false);

// 	GetCharacterMovement()->JumpZVelocity = 600.0f;
// 	GetCharacterMovement()->bUseRVOAvoidance = true;
// 	GetCharacterMovement()->AvoidanceConsiderationRadius = 100;

	ConstructorHelpers::FClassFinder <AAIController> tempcon(TEXT("Class'/Script/AIModule.DetourCrowdAIController'"));
	if (tempcon.Succeeded())
	{
		AIControllerClass = tempcon.Class;

	}
	GetCharacterMovement()->JumpZVelocity = 600;
	GetCharacterMovement()->AirControl = 1;
}

// Called when the game starts or when spawned
void ASH_Enemy::BeginPlay()
{
	Super::BeginPlay();
	setPhysicsData();
}

// Called every frame
void ASH_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (fsm->mState == EEnemyState::Die || fsm->mState == EEnemyState::Down)
	{
		/*DeadBlock->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);*/
		GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("spine_03"), false);
	}
	else
	{
		/*DeadBlock->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
		GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("spine_03"), true);
	}

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


void ASH_Enemy::AddTarget(AActor* actor) //타겟 어레이에 액터를 추가한다.
{
	fsm->targets.AddUnique(actor);
}

void ASH_Enemy::RemoveTarget(AActor* actor) // 타겟 어레이에 액터를 제거한다.
{
	fsm->targets.Remove(actor);
}

void ASH_Enemy::setPhysicsData()
{

	FPhysicalAnimationData PhysicalAnimDate;
	PhysicalAnimDate.bIsLocalSimulation = true;
	PhysicComp->StrengthMultiplyer = 1.0f;
	PhysicalAnimDate.OrientationStrength = 5000.0f;
	PhysicalAnimDate.AngularVelocityStrength = 100.0f;
	PhysicalAnimDate.PositionStrength = 5000.0f;
	PhysicalAnimDate.VelocityStrength = 100.0f;
	PhysicalAnimDate.MaxAngularForce = 100.0f;
	PhysicalAnimDate.MaxLinearForce = 100.0f;
	PhysicComp->SetSkeletalMeshComponent(GetMesh());
	PhysicComp->ApplyPhysicalAnimationSettingsBelow(TEXT("spine_03"), PhysicalAnimDate, true);
	GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("spine_03"), true);
}