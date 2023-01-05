// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SH_Enemy.generated.h"

UCLASS()
class DOGFIGHT_API ASH_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASH_Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
	class USH_EnemyFSM* fsm; // 적  AI 관리하는 FSM액터 컴포넌트

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* compAttack;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* compAttack2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* compMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class USkeletalMesh*> randmesh;

	UFUNCTION()
	void attackBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	bool isAttackBoxOverlap ;

	UPROPERTY()
	class ASH_Enemy* currEnemy;

	UPROPERTY()
	class ASH_Player* player;

	float currentTime = 0;
	float delayTime = 2.0f;

	UFUNCTION()
	void GetEnemyWeapon(UStaticMesh* WeaponMesh, FString soketname);
};