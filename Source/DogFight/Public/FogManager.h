// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FogManager.generated.h"

UCLASS()
class DOGFIGHT_API AFogManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFogManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



public:
	//테스트
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMeshAlive;

	//현재시간
	UPROPERTY(EditAnywhere)
	float currentTime = 0;

	//AliveZone 원 크기
	UPROPERTY(EditAnywhere)
	float scale = 80;

	//AliveZone 원 줄어드는 시간
	UPROPERTY(EditAnywhere)
	float scaleSpeed = 0.1f; //0.008f

	//AliveZone 원 반지름
	UPROPERTY(EditAnywhere)
	float r;

	//AliveZone 범위. 원점에서 반지름까지 위치? 거리? 
	UPROPERTY(EditAnywhere)
	float aliveRange;

	//액터들의 위치
	UPROPERTY(EditAnywhere)
	FVector actorPos;

	//AlvieZone 원점(위치)과 액터들(위치)의 거리. 간격
	UPROPERTY(EditAnywhere)
	float moveDist;

public:
	void AliveZone();
	void FindActorPos();
	
	bool isAlive = true;
	float delayTime = 0;
	

};