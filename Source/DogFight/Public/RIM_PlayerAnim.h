// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RIM_PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API URIM_PlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	


public:
	//�÷��̾� �̵� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float speed = 0;

	//�� ������ ���ŵǴ� �Լ�
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


};
//