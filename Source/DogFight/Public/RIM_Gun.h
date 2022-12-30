// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RIM_Gun.generated.h"

UCLASS()
class DOGFIGHT_API ARIM_Gun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARIM_Gun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EnableInput(class APlayerController* PlayerController) override;

public:
	//�ٴڿ� �ִ� �� �浹 ������Ʈ(�浹ü). USphereComponent ��� ���� �߰�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* compCollision;

	//�ٴڿ� �ִ� �� �ܰ� ������Ʈ. UStaticMeshComponent ��� ���� �߰�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* compMeshWeaponGun;

public:
	//[�ٴڿ� �ִ� �Ѱ� �÷��̾� �浹 �� �Լ� ����]
	UFUNCTION()
	void collisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void collisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//[�÷��̾�???]
	UPROPERTY()
	class ARIM_Player* RIM_Player;

	//[�ٴڿ� �ִ� �� �� ���̰� �ϴ� �Լ�]
	void getGun();

	//virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	//virtual void NotifyActorEndOverlap(AActor* OtherActor) override;



};