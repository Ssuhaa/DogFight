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
	//바닥에 있는 총 충돌 컴포넌트(충돌체). USphereComponent 멤버 변수 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* compCollision;

	//바닥에 있는 총 외관 컴포넌트. UStaticMeshComponent 멤버 변수 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* compMeshWeaponGun;

public:
	//[바닥에 있는 총과 플레이어 충돌 시 함수 구현]
	UFUNCTION()
	void collisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void collisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//[플레이어???]
	UPROPERTY()
	class ARIM_Player* RIM_Player;

	//[바닥에 있는 총 안 보이게 하는 함수]
	void getGun();

	//virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	//virtual void NotifyActorEndOverlap(AActor* OtherActor) override;



};