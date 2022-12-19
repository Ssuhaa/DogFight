// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RIM_Baseballbat.generated.h"

UCLASS()
class DOGFIGHT_API ARIM_Baseballbat : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARIM_Baseballbat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//[야구망방이 스켈레탈메시 컴포넌트 멤버 변수 추가]
	//야구망방이 스켈레탈 메시
	UPROPERTY(VisibleAnywhere, Category = BaseballbatMesh)
	class USkeletalMeshComponent* baseballbatMeshComp;

	//[충돌]
	//충돌체, 충돌 컴포넌트. USphereComponent 멤버 변수 추가
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* collisionComp;
};