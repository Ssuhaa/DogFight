// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RIM_Arrow.generated.h"

UCLASS()
class DOGFIGHT_API ARIM_Arrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARIM_Arrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//�߻�ü�� �̵�, �߻�ü ������ ���� ����� �����ϴ� ������Ʈ. UProjectileMovementComponent ��� ���� �߰�
	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* movementComp;

	//�浹ü, �浹 ������Ʈ. USphereComponent ��� ���� �߰�
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* collisionComp;

	//�ܰ� ������Ʈ. UStaticMeshComponent ��� ���� �߰�
	UPROPERTY(VisibleAnywhere, Category = BodyMesh)
	class UStaticMeshComponent* bodyMeshComp;
};
