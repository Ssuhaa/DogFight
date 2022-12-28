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

	//[�浹ü ������Ʈ �߰�]
	//�浹ü ����ϱ�. �浹ü �ν��Ͻ� ����ϰ� collisionComp ������ ���
	//�浹ü�� ��Ʈ ������Ʈ�� ���
	//�浹�������� ����. �浹ü�� �浹 ���������� BlockAll�� ����. ��� ��ü�� �ε��� ƨ�� �� �ְ� �ϱ�
	collisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = collisionComp;
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));



	//[����ƽ�޽� ������Ʈ �߰�]
	//�ܰ� ������Ʈ ����ϱ�. UStaticMeshComponent �ν��Ͻ� ����� bodyMeshComp ��� ������ �Ҵ�
	//�θ� ������Ʈ ����. �θ� ������Ʈ�� coolisionComp ����
	//�浹 ��Ȱ��ȭ
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("itemGunMeshComp"));
	bodyMeshComp->SetupAttachment(collisionComp);
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//ConstructorHelpers::FObjectFinder<UStaticMesh> tempmesh(TEXT(""));
// 	if (tempmesh.Succeeded())
// 	{
// 		bodyMeshComp->Set
// 	}


	//[movementComp ������Ʈ �߰�] //�ڡڡ��ʿ� ���� �� ��ü ����
	//movementComp ������Ʈ
	//movement ������Ʈ�� �����ų ������Ʈ ����
	//movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	//movementComp->SetUpdatedComponent(collisionComp);

	//[movementComp ������Ʈ �ʱⰪ ����] //�ڡڡ��ʿ� ���� �� ��ü ����
	//�ʱ�ӵ�. InitialSpeed �Ӽ� �̿�
	//�ִ�ӵ�. MaxSpeed �Ӽ� �̿�
	//�ݵ�����. bShouldBounce �Ӽ� �̿�. true �Ҵ�
	//�ݵ� ��. �ݵ��� �ִٸ� ź���� ��� ���� ���� Bounciness �Ӽ� �̿�
	//���� �ð� �ֱ�
	//movementComp->InitialSpeed = 500; //�ڡڡ����� ���� �ʿ�
	//movementComp->MaxSpeed = 500; //�ڡڡ����� ���� �ʿ�
	//movementComp->bShouldBounce = true;
	//movementComp->Bounciness = 0.2f; //�ڡڡ����� ���� �ʿ�
	//InitialLifeSpan = 30.0f; //�ڡڡ����� ���� �ʿ�


	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &ARIM_Gun::collisionBeginOverlap);
	collisionComp->OnComponentEndOverlap.AddDynamic(this, &ARIM_Gun::collisionEndOverlap);
	
	

}

// Called when the game starts or when spawned
void ARIM_Gun::BeginPlay()
{
	Super::BeginPlay();
	//[�ٴڿ� �ִ� �Ѱ� �÷��̾� �浹 �̺�Ʈ �Լ� ���ε�/ȣ��]

}

// Called every frame
void ARIM_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARIM_Gun::EnableInput(APlayerController* PlayerController)
{
	Super::EnableInput(PlayerController);

	PlayerController->InputComponent->BindAction(TEXT("PunchGrab"), IE_Pressed, this, &ARIM_Gun::getGun);
}


//[�ٴڿ� �ִ� �Ѱ� �÷��̾� �浹 �̺�Ʈ �Լ� ����]
void ARIM_Gun::collisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("11111111111111"));
	RIM_Player = Cast<ARIM_Player>(OtherActor);
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
}

void ARIM_Gun::collisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//AActor* RIM_Player = Cast<ARIM_Player>(OtherActor);
	//DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ARIM_Gun::getGun()
{
	UE_LOG(LogTemp, Warning, TEXT("22222222"));
	RIM_Player->VisibleGun();
	Destroy();
}

void ARIM_Gun::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("5555555555"));
	RIM_Player = Cast<ARIM_Player>(OtherActor);
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ARIM_Gun::NotifyActorEndOverlap(AActor* OtherActor)
{
	AActor* RIM_Player = Cast<ARIM_Player>(OtherActor);
	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}