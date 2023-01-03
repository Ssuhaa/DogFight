// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class DOGFIGHT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void EnableInput(class APlayerController* PlayerController) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* compCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* compMesh;

	UFUNCTION()
	void collisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void GetWeapon();

	UFUNCTION()
	void BindGetWeapon();

	UPROPERTY()
	class ARIM_Player* player;

	UPROPERTY()
	class ASH_Enemy* Enemy;

	UPROPERTY()
	class AActor* overlapActor;
	
	FString Soketname;
};
