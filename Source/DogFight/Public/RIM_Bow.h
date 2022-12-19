// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RIM_Bow.generated.h"

UCLASS()
class DOGFIGHT_API ARIM_Bow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARIM_Bow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//[È° ½ºÄÌ·¹Å»¸Þ½Ã ÄÄÆ÷³ÍÆ® ¸â¹ö º¯¼ö Ãß°¡]
	//È° ½ºÄÌ·¹Å» ¸Þ½Ã
	UPROPERTY(VisibleAnywhere, Category = BowMesh)
	class USkeletalMeshComponent* bowMeshComp;
};
