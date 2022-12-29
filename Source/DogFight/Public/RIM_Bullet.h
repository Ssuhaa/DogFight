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
	//�浹 ������Ʈ(�浹ü). USphereComponent ��� ���� �߰�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Collision) //�ڡڡ�ī�װ� �ʿ� ������ ���� ����
	class USphereComponent* compCollision;

	//�ܰ� ������Ʈ. UStaticMeshComponent ��� ���� �߰�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=BodyMesh) //�ڡڡ�ī�װ� �ʿ� ������ ���� ����
	class UStaticMeshComponent* compMeshWeaponBullet;
	 
	//�߻�ü�� �̵�, �߻�ü ������ ���� ����� �����ϴ� ������Ʈ. UProjectileMovementComponent ��� ���� �߰�
	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* compMovement;

};
