// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KillZone.generated.h"

UCLASS()
class DOGFIGHT_API AKillZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKillZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* compBox;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY()
	class ARIM_Player* Player;

	UPROPERTY()
	class ASH_Enemy* Enemy;

	UPROPERTY()
	class AWeapon* weapon;

	float delayDieTime;




};
