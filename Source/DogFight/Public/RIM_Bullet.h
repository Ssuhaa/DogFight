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
	//총알 충돌 컴포넌트(충돌체). USphereComponent 멤버 변수 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* compCollision;

	//총알 외관 컴포넌트. UStaticMeshComponent 멤버 변수 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* compMeshWeaponBullet;
	 
	//총알 발사체의 이동, 발사체 형태의 물리 기능을 제고하는 컴포넌트. UProjectileMovementComponent 멤버 변수 추가
	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* compMovement;

public:
	//[델리케이트] ★★★ 완벽하게 이해하지 못 함
	//[[2]총알 콜리전과 에너미가 충돌 시 실행 될 함수 선언]
	UFUNCTION()
	void collisionBulletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	class ASH_Enemy* Enemy;
};
