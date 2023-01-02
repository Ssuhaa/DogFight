// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_Gun.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "RIM_Player.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ARIM_Gun::ARIM_Gun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//[�ٴڿ� �ִ� �� �浹 ������Ʈ(�浹ü) �߰�]
	//�浹ü ����ϱ�. �浹ü �ν��Ͻ� ����ϰ� compCollision ������ ���
	//�浹ü�� ��Ʈ ������Ʈ�� ���
	//�浹�������� ����. �浹ü�� �浹 ���������� BlockAll�� ����. ��� ��ü�� �ε��� ƨ�� �� �ְ� �ϱ�
	compCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = compCollision;
	compCollision->SetCollisionProfileName(TEXT("BlockAll"));



	//[�ٴڿ� �ִ� �� ����ƽ�޽� ������Ʈ �߰�]
	//�ܰ� ������Ʈ ����ϱ�. UStaticMeshComponent �ν��Ͻ� ����� compMeshWeaponGun ��� ������ �Ҵ�
	//�θ� ������Ʈ ����. �θ� ������Ʈ�� coolisionComp ����
	//�浹 ��Ȱ��ȭ
	compMeshWeaponGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponGunMeshComp"));
	compMeshWeaponGun->SetupAttachment(compCollision);
	compMeshWeaponGun->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//[�ٴڿ� �ִ� �� ����ƽ�޽�. �� �߰�]
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempItemGunMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'")); 
	if (TempItemGunMesh.Succeeded())
	{
		compMeshWeaponGun->SetStaticMesh(TempItemGunMesh.Object);
		compMeshWeaponGun->SetRelativeLocationAndRotation(FVector(0, 0, -20), FRotator(0, 0, 0));
		compMeshWeaponGun->SetRelativeScale3D(FVector(0.35f));
	}

}

// Called when the game starts or when spawned
void ARIM_Gun::BeginPlay()
{
	Super::BeginPlay();
	
	//[�ٴڿ� �ִ� �Ѱ� �÷��̾� �浹 ��...?]
	compCollision->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Gun::collisionBeginOverlap);
	compCollision->OnComponentEndOverlap.AddDynamic(this, &ARIM_Gun::collisionEndOverlap);


}

// Called every frame
void ARIM_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

//[???]
void ARIM_Gun::EnableInput(APlayerController* PlayerController)
{
	Super::EnableInput(PlayerController);

	PlayerController->InputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, &ARIM_Gun::getGun);
}




//[�ٴڿ� �ִ� �Ѱ� �÷��̾� �浹 �� �Լ� ����]
void ARIM_Gun::collisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //���� �浹�Ѱ� ���� Ȯ���ϴ°� �־���Ѵ�
	RIM_Player = Cast<ARIM_Player>(OtherActor);
	if (RIM_Player != nullptr)
	{
		EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
}

void ARIM_Gun::collisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	RIM_Player = Cast<ARIM_Player>(OtherActor);
}




//[�ٴڿ� �ִ� �� �� ���̰� �ϴ� �Լ� ����]
void ARIM_Gun::getGun()
{
	//�÷��̾ ��� �ִ� ���� ���̸�
	//�ٴڿ� �ִ� �� �ı�(�� ����)
	RIM_Player->VisibleGun();
	Destroy();
}

/*
void ARIM_Gun::NotifyActorBeginOverlap(AActor* OtherActor)
{
	RIM_Player = Cast<ARIM_Player>(OtherActor);
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ARIM_Gun::NotifyActorEndOverlap(AActor* OtherActor)
{
	AActor::EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}
*/