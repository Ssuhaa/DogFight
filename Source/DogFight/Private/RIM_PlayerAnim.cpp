// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_PlayerAnim.h"
#include "RIM_Player.h"

void URIM_PlayerAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay(); //★★★부모의 함수 호출. 왜?

	//RIM_Player 저장하기
	//1.소유 폰 가져오기
	APawn* pawn = TryGetPawnOwner(); //블루프린트의 Try Get Pawn Owner 노드 

	//2.플레이어로 캐스팅하기
	 player = Cast<ARIM_Player>(pawn); //인클루드

	
}

void URIM_PlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds); //★★★부모의 함수 호출. 왜?
	
// 	//RIM_Player 저장하기
// 	//1.소유 폰 가져오기
// 	auto ownerPawn = TryGetPawnOwner();
// 
// 	//2.플레이어로 캐스팅하기
// 	auto player = Cast<ARIM_Player>(ownerPawn); //인클루드
	
	//캐스팅 성공했다면
	if(player)
	{
		//3. 이동 속도 가져오기
		FVector velocity = player->GetVelocity();
		//4. 플레이어의 앞방향(전방 벡터) 가져오기
		FVector forwardVector = player->GetActorForwardVector();
		//5. 두 벡터를 내적한 값을 dirV에 세팅하기(할당하기)
		dirV = FVector::DotProduct(forwardVector, velocity);

		//플레이어의 좌우방향 가져오기
		FVector rightVector = player->GetActorRightVector();
		//두 벡터를 내적한 값을 dirHdp 세팅하기(할당하기)
		dirH = FVector::DotProduct(rightVector, velocity);


	}
}

//[섞인 애니메이션 ---> 플레이어 공격 몽타주 애니메이션 재생 함수 구현] //★★★???
void URIM_PlayerAnim::PlayPlayerAnim(FString Name, int32 index)
{
	FString sectionName = FString::Printf(TEXT("%s%d"), *Name, index); //섹션네임(한글+숫자 조합)
	player->PlayAnimMontage(playerMontage, 1.0f, FName(*sectionName)); //플레이어에서 몽타주를 가져 온다???
}

//[섞이지 않는 애니메이션 ---> 플레이어 공격 몽타주 애니메이션 재생 함수 구현] //★★★???
void URIM_PlayerAnim::PlayPlayerTwoAnim(FString Name, int32 index)
{
	FString sectionName = FString::Printf(TEXT("%s%d"), *Name, index); //섹션네임(한글+숫자 조합)
	player->PlayAnimMontage(playerMontageTwo, 1.0f, FName(*sectionName)); //플레이어에서 몽타주를 가져 온다???
}