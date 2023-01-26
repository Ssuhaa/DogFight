// Fill out your copyright notice in the Description page of Project Settings.


#include "FogManager.h"
#include <Components/StaticMeshComponent.h>
#include <Components/ShapeComponent.h>
#include "KillZone.h"
#include <GameFramework/Actor.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/Character.h>
#include "Weapon.h"

// Sets default values
AFogManager::AFogManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	compMeshAlive = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AliveMesh"));
	RootComponent = compMeshAlive;
	compMeshAlive->SetRelativeLocation(FVector(110, -400, 0));
	compMeshAlive->SetRelativeScale3D(FVector(scale));
	compMeshAlive->SetCollisionProfileName(TEXT("OverlapAll"));
	//메시, 머테리얼 블루프린트에서 넣음
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMeshAlive(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (tempMeshAlive.Succeeded())
	{
		compMeshAlive->SetStaticMesh(tempMeshAlive.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterial> tempM(TEXT("Material'/Game/InfinityBladeEffects/Effects/FX/M_FogPlane02.M_FogPlane02'"));
	if (tempM.Succeeded())
	{
		compMeshAlive->SetMaterial(0, tempM.Object);
	}
}

// Called when the game starts or when spawned
void AFogManager::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AFogManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AliveZone();

}

void AFogManager::AliveZone()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	
	if (scale < 15)
	{
		scaleSpeed = 0;
	}
	else
	{
		if (currentTime < 30)
		{
			scale -= scaleSpeed;
		}
		else
		{
			delayTime += GetWorld()->DeltaTimeSeconds;
			if (delayTime > 10)
			{
				currentTime = 0;
				delayTime = 0;
			}
		}
	}

	compMeshAlive->SetRelativeScale3D(FVector(scale));

	r = scale * 100 / 2; //AliveZone 원 반지름
	aliveRange = GetActorLocation().Y + r; //AliveZone 범위. 원점에서 반지름까지 위치? 거리? ★★★400 이 아니라 위치 Y 값!!!
	FindActorPos();
}


void AFogManager::FindActorPos()
{
	TArray<AActor*> Characters; //지역변수. 아웃라이너에 있는 '모든 액터'의 배열 ★★★필요한 것인지 잘 모르겠다
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), Characters); //찾고자 하는 클래스를 상속한 액터를 모두 다 찾아준다

	TArray<AActor*> Weapons;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeapon::StaticClass(), Weapons);

	//찾아서 어레이에 일일히 담는게 아니라 characterSpwan에 걸어놓은 딜리게이트로 배열을 저장하고싶다.

	for (int32 i = 0; i < Weapons.Num(); i++)
	{
		Characters.Add(Weapons[i]);
	}

	for (int32 i = 0; i < Characters.Num(); i++)
	{
		actorPos = Characters[i]->GetActorLocation(); //캐릭터(액터)들의 위치
		moveDist = FVector::Distance(this->GetActorLocation(), actorPos); //AlvieZone 원점(위치)과 액터들(위치)의 거리. 간격

		if (moveDist > aliveRange) //액터(플레이어, 에너미 등)가 Alive 영역 안에 없으면, 원 밖에 있으면
		{
			isAlive = false; //Alive 아님 ---> Kill
		}
		else //액터가 Alive 영역 안에 있다
		{
			isAlive = true; //Alive 맞음
		}
	}
}







