// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>
#include "RIM_Player.h"
#include "SH_Enemy.h"
#include "SH_EnemyFSM.h"
#include "EnemyAnim.h"
#include <Engine/SkeletalMeshSocket.h>
#include "SH_Player.h"
#include <Engine/SkeletalMesh.h>


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//WeaponMesh
	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = compMesh;
	compMesh->SetupAttachment(compCollision);
	compMesh->SetCollisionProfileName(TEXT("BlockAll"));
	compMesh->SetSimulatePhysics(true);

	//WeaponCollison
	compCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	compCollision->SetupAttachment(compMesh);
	compCollision->SetBoxExtent(FVector(40));
	compCollision->SetCollisionProfileName(TEXT("OverlapAll"));

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	compCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::collisionBeginOverlap);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::EnableInput(class APlayerController* PlayerController)
{
	Super::EnableInput(PlayerController);
	PlayerController->InputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, &AWeapon::BindGetWeapon);
}

void AWeapon::collisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	overlapActor = OtherActor;
	if (OtherActor->GetName().Contains(TEXT("Player")))
	{
		EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
	else if (OtherActor->GetName().Contains(TEXT("Enemy")))
	{
		BindGetWeapon();
	}
}

void AWeapon::BindGetWeapon()
{
	
	GetWeapon();
}

void AWeapon::GetWeapon()
{
	player = Cast<ARIM_Player>(overlapActor);
	if (player != nullptr)
	{	

		//만약 플레이어의 Gun or Lollipop 메시가 보이면(플레이어가 Gun, Lollipop 을 들고 있다)
		//플레이어의 Gun 메시에서 IsVisible 함수(플레이어의 Gun 메시 보이게 하는 함수)가 true 이거나
		//플레이어의 LolliPop 메시에서 IsVisible 함수(플레이어의 LolliPop 메시 보이게 하는 함수)가 true 이면
		//함수를 나간다. 실행하지 않는다
		if (player->compMeshGun->IsVisible() == true || player->compMeshLollipop->IsVisible() == true) return; 

			if (Soketname == TEXT("Gun")) //소켓 이름의 텍스트가 Gun 이면
			{
				UE_LOG(LogTemp, Error, TEXT("Player Gun Pickup!")); //확인용 텍스트 출력
					
 				//플레이어 애니메이션 몽타주 중 '픽업' 애니메이션 재생 ★★★오류. 나중에 하기
// 				URIM_PlayerAnim* animPlayer = Cast<URIM_PlayerAnim>(GetMesh()->GetAnimInstance());
// 				animPlayer->PlayPlayerAnim(TEXT("Pickup"), 0);

				player->VisibleGun(); //플레이어의 Gun 메시가 보이게 한다. 플레이어가 Gun 을 들고 있게 한다. ---> 플레이어 Gun 비저블 함수 호출
				//player->isInputPunchGrab = false; //InputPunchGrab 버튼 변수 초기화(false) ★★★삭제해도 될 듯
				Destroy(); //바닥의 Gun 은 파괴한다
			}
			else if (Soketname == TEXT("Lollipop")) //소켓 이름의 텍스트가 Lollipop 이면
			{
				UE_LOG(LogTemp, Error, TEXT("Player Lollipop Pickup!")); //확인용 텍스트 출력
					
 				//플레이어 애니메이션 몽타주 중 '픽업' 애니메이션 재생 ★★★오류. 나주에 하기
// 				URIM_PlayerAnim* animPlayer = Cast<URIM_PlayerAnim>(GetMesh()->GetAnimInstance());
// 				animPlayer->PlayPlayerAnim(TEXT("Pickup"), 0);

				player->VisibleLollipop(); //플레이어의 Lollipop 메시가 보이게 한다. 플레이어가 Lollipop 을 들고 있게 한다. ---> 플레이어 Lollipop 비저블 함수 호출
				//player->isInputPunchGrab = false; //InputPunchGrab 버튼 변수 초기화(false) ★★★삭제해도 될 듯
				Destroy(); //바닥의 Lollipop 은 파괴한다
			}
// 			else ★★★else 부분 삭제해도 될 듯
// 			{
// 				player->isInputPunchGrab = false;
// 				return;
// 			}
	}

	Enemy = Cast<ASH_Enemy>(overlapActor);
	if (Enemy != nullptr)
	{
		if (Enemy->fsm->mState == EEnemyState::Damage || Enemy->fsm->mState == EEnemyState::Down || Enemy->fsm->mState == EEnemyState::Die|| Enemy->fsm->mState == EEnemyState::Attack) return;
		if(Enemy->fsm->anim->isGunget == false && Enemy->fsm->anim->isLollipopget == false)
		{
			Enemy->GetEnemyWeapon(compMesh->GetStaticMesh(), Soketname);
			Enemy->fsm->removeWeaponArray();
			Enemy->fsm->stateChange(EEnemyState::Pickup);
			if (Soketname == TEXT("Lollipop"))
			{
				Enemy->fsm->anim->isLollipopget = true;
			}
			else if((Soketname == TEXT("Gun")))
			{
				Enemy->fsm->anim->isGunget = true;
			}
			Destroy();
		}
	}

}
