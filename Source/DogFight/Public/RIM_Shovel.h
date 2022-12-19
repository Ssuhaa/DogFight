// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RIM_Shovel.generated.h"

UCLASS()
class DOGFIGHT_API ARIM_Shovel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARIM_Shovel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//[»ð ½ºÄÌ·¹Å»¸Þ½Ã ÄÄÆ÷³ÍÆ® ¸â¹ö º¯¼ö Ãß°¡]
	//»ð ½ºÄÌ·¹Å» ¸Þ½Ã
	UPROPERTY(VisibleAnywhere, Category = ShovelMesh)
	class USkeletalMeshComponent* shovelMeshComp;

	//[Ãæµ¹]
	//Ãæµ¹Ã¼, Ãæµ¹ ÄÄÆ÷³ÍÆ®. USphereComponent ¸â¹ö º¯¼ö Ãß°¡
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* collisionComp;
};
