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
	virtual void NativeBeginPlay() override; //�ڡڡڿ� �ʿ�����? ��� ã�Ƽ� �ִ� ������ ��� �ȳ�.
	
	//NativeUpdateAnimation() ---> �������Ʈ�� Event Blueprint Update Animation ���� �����Ǵ� �Լ�
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //�ڡڡڿ� �ʿ�����? ��� ã�Ƽ� �ִ� ������ ��� �ȳ�.

public:
	//�÷��̾� ����
	UPROPERTY()
	class ARIM_Player* player;

	//���ǿ� ���ϸ� ��, �ڸ� �Ǻ��ϴ� ����. (å�� ���ϸ�)�÷��̾� ��, �� �̵� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float dirV = 0;

	//��, �� �̵� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dirH = 0;

	//����. ���߿� �ִ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAir = false;

	//"���� �ִϸ��̼�" �÷��̾� �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditAnywhere)
	class UAnimMontage* playerMontage;

	//"������ ���� �ִϸ��̼�" �÷��̾� �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditAnywhere)
	class UAnimMontage* playerMontageTwo;
 
 	//"���� �ִϸ��̼�" �÷��̾� ���� �ִϸ��̼� ��� �Լ�
 	UFUNCTION()
 	void PlayPlayerAnim(FString Name,int32 index); //������ ���� �̸��� ���� FName Ÿ�� sectionName�� �Ű������� ���� �Լ�

	//"������ ���� �ִϸ��̼�" �÷��̾� ���� �ִϸ��̼� ��� �Լ�
	UFUNCTION()
	void PlayPlayerTwoAnim(FString Name, int32 index); //������ ���� �̸��� ���� FName Ÿ�� sectionName�� �Ű������� ���� �Լ�



};
