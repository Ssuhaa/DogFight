// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_PlayerAnim.h"
#include "RIM_Player.h"


void URIM_PlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	;
	//�÷��̾��� �̵� �ӵ��� ������ speed�� �Ҵ��ϰ� �ʹ�
	//1.���� �� ������
	auto ownerPawn = TryGetPawnOwner();
	//2.�÷��̾�� ĳ�����ϱ�
	auto player = Cast<ARIM_Player>(ownerPawn);
	//ĳ���� �����ߴٸ�
	if(player)
	{
	//3. �̵� �ӵ��� �ʿ�
	FVector velocity = player->GetVelocity();
	//4. �÷��̾��� ���� ���Ͱ� �ʿ�
	FVector forwardVector = player->GetActorForwardVector();
	//5. speed�� ��(����) �Ҵ��ϱ�
	speed = FVector::DotProduct(forwardVector, velocity);
	}
}
