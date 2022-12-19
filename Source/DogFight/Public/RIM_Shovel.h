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
	//[�� ���̷�Ż�޽� ������Ʈ ��� ���� �߰�]
	//�� ���̷�Ż �޽�
	UPROPERTY(VisibleAnywhere, Category = ShovelMesh)
	class USkeletalMeshComponent* shovelMeshComp;

	//[�浹]
	//�浹ü, �浹 ������Ʈ. USphereComponent ��� ���� �߰�
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* collisionComp;
};
