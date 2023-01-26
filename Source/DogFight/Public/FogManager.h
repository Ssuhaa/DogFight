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
	//�׽�Ʈ
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMeshAlive;

	//����ð�
	UPROPERTY(EditAnywhere)
	float currentTime = 0;

	//AliveZone �� ũ��
	UPROPERTY(EditAnywhere)
	float scale = 80;

	//AliveZone �� �پ��� �ð�
	UPROPERTY(EditAnywhere)
	float scaleSpeed = 0.1f; //0.008f

	//AliveZone �� ������
	UPROPERTY(EditAnywhere)
	float r;

	//AliveZone ����. �������� ���������� ��ġ? �Ÿ�? 
	UPROPERTY(EditAnywhere)
	float aliveRange;

	//���͵��� ��ġ
	UPROPERTY(EditAnywhere)
	FVector actorPos;

	//AlvieZone ����(��ġ)�� ���͵�(��ġ)�� �Ÿ�. ����
	UPROPERTY(EditAnywhere)
	float moveDist;

public:
	void AliveZone();
	void FindActorPos();
	
	bool isAlive = true;
	float delayTime = 0;
	

};