// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SH_EnemyFSM.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=FSM)
	EEnemyState animState; //상태머신기억변수
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=FSM)
	bool bAttackPlay = false;
	UFUNCTION(BlueprintImplementableEvent, Category = FSM)
	void PlayDamagaAnim(FName sectionName);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	bool bDieDone =false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	bool isLollipopget =false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	bool isGunget = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	bool isShovel = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	bool isTennis = false;


	UFUNCTION()
	void AnimNotify_Attack();
	
	UPROPERTY()
	class ASH_Enemy* me;
};
