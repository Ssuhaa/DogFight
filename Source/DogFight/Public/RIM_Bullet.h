// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RIM_Bullet.generated.h"

UCLASS()
class DOGFIGHT_API ARIM_Bullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARIM_Bullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//�Ѿ� �浹 ������Ʈ(�浹ü). USphereComponent ��� ���� �߰�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* compCollision;

	//�Ѿ� �ܰ� ������Ʈ. UStaticMeshComponent ��� ���� �߰�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* compMeshWeaponBullet;
	 
	//�Ѿ� �߻�ü�� �̵�, �߻�ü ������ ���� ����� �����ϴ� ������Ʈ. UProjectileMovementComponent ��� ���� �߰�
	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* compMovement;

public:
	//[��������Ʈ] �ڡڡ� �Ϻ��ϰ� �������� �� ��
	//[[2]�Ѿ� �ݸ����� ���ʹ̰� �浹 �� ���� �� �Լ� ����]
	UFUNCTION()
	void collisionBulletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	class ASH_Enemy* Enemy;
};
