// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterSpawn.generated.h"

UCLASS()
class DOGFIGHT_API ACharacterSpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterSpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class ASH_Enemy>> EnemyArray;

	UPROPERTY(EditAnywhere)
	TArray<class USceneComponent*> SpawnPosition;

};
