// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LollipopWeapon.h"
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

	UPROPERTY(BlueprintReadOnly)
	TArray<class AWeapon*> ItemArray;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ALollipopWeapon> Loliipop;

	float CurrentTime =0;
	float SpawnTime = 5;


};
