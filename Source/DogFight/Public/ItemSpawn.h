// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LollipopWeapon.h"
#include "SH_EnemyFSM.h"
#include "ItemSpawn.generated.h"

UCLASS()
class DOGFIGHT_API AItemSpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* itemSpawnLocation ;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<TSubclassOf<class AWeapon>> ItemArray;

	float CurrentTime =10;
	UPROPERTY(EditAnywhere)
	float SpawnTime = 10;

	void CreateWeapon(int32 index, FVector SpwanLocation, FRotator SpwanRotation);
	void DeleteWeapon(AActor* actor);

	FTargetinterfaceDel createWeapon;
	FTargetinterfaceDel deleteWeapon;

};
