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

public:
	//�浹 ������Ʈ(�浹ü). USphereComponent ��� ���� �߰�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
	class UBoxComponent* collisionComp;

	//�ܰ� ������Ʈ. UStaticMeshComponent ��� ���� �߰�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = BodyMesh)
	class UStaticMeshComponent* bodyMeshComp;

	//�߻�ü�� �̵�, �߻�ü ������ ���� ����� �����ϴ� ������Ʈ. UProjectileMovementComponent ��� ���� �߰� //�ڡڡ��ʿ� ���� �� ��ü ����
	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* movementComp;

};


