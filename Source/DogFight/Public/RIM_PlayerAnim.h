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
	virtual void NativeBeginPlay() override; //★★★왜 필요하지? 어떻게 찾아서 넣는 것인지 기억 안남.
	
	//NativeUpdateAnimation() ---> 블루프린트의 Event Blueprint Update Animation 노드와 대응되는 함수
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //★★★왜 필요하지? 어떻게 찾아서 넣는 것인지 기억 안남.

public:
	//플레이어 액터
	UPROPERTY()
	class ARIM_Player* player;

	//강의에 의하면 앞, 뒤를 판별하는 변수. (책에 의하면)플레이어 앞, 뒤 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float dirV = 0;

	//좌, 우 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dirH = 0;

	//점프. 공중에 있는지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAir = false;

	//"섞인 애니메이션" 플레이어 애니메이션 몽타주
	UPROPERTY(EditAnywhere)
	class UAnimMontage* playerMontage;

	//"섞이지 않은 애니메이션" 플레이어 애니메이션 몽타주
	UPROPERTY(EditAnywhere)
	class UAnimMontage* playerMontageTwo;
 
 	//"섞인 애니메이션" 플레이어 공격 애니메이션 재생 함수
 	UFUNCTION()
 	void PlayPlayerAnim(FString Name,int32 index); //저장할 섹션 이름을 받을 FName 타입 sectionName을 매개변수로 갖는 함수

	//"섞이지 않은 애니메이션" 플레이어 공격 애니메이션 재생 함수
	UFUNCTION()
	void PlayPlayerTwoAnim(FString Name, int32 index); //저장할 섹션 이름을 받을 FName 타입 sectionName을 매개변수로 갖는 함수



};
