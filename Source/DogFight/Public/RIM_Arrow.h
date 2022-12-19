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
	//발사체의 이동, 발사체 형태의 물리 기능을 제고하는 컴포넌트. UProjectileMovementComponent 멤버 변수 추가
	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* movementComp;

	//충돌체, 충돌 컴포넌트. USphereComponent 멤버 변수 추가
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* collisionComp;

	//외관 컴포넌트. UStaticMeshComponent 멤버 변수 추가
	UPROPERTY(VisibleAnywhere, Category = BodyMesh)
	class UStaticMeshComponent* bodyMeshComp;
};
