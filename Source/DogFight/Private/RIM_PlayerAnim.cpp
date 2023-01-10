// Fill out your copyright notice in the Description page of Project Settings.


#include "RIM_PlayerAnim.h"
#include "RIM_Player.h"

void URIM_PlayerAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay(); //�ڡڡںθ��� �Լ� ȣ��. ��?

	//RIM_Player �����ϱ�
	//1.���� �� ��������
	APawn* pawn = TryGetPawnOwner(); //�������Ʈ�� Try Get Pawn Owner ��� 

	//2.�÷��̾�� ĳ�����ϱ�
	 player = Cast<ARIM_Player>(pawn); //��Ŭ���

	
}

void URIM_PlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds); //�ڡڡںθ��� �Լ� ȣ��. ��?
	
// 	//RIM_Player �����ϱ�
// 	//1.���� �� ��������
// 	auto ownerPawn = TryGetPawnOwner();
// 
// 	//2.�÷��̾�� ĳ�����ϱ�
// 	auto player = Cast<ARIM_Player>(ownerPawn); //��Ŭ���
	
	//ĳ���� �����ߴٸ�
	if(player)
	{
		//3. �̵� �ӵ� ��������
		FVector velocity = player->GetVelocity();
		//4. �÷��̾��� �չ���(���� ����) ��������
		FVector forwardVector = player->GetActorForwardVector();
		//5. �� ���͸� ������ ���� dirV�� �����ϱ�(�Ҵ��ϱ�)
		dirV = FVector::DotProduct(forwardVector, velocity);

		//�÷��̾��� �¿���� ��������
		FVector rightVector = player->GetActorRightVector();
		//�� ���͸� ������ ���� dirHdp �����ϱ�(�Ҵ��ϱ�)
		dirH = FVector::DotProduct(rightVector, velocity);


	}
}

//[���� �ִϸ��̼� ---> �÷��̾� ���� ��Ÿ�� �ִϸ��̼� ��� �Լ� ����] //�ڡڡ�???
void URIM_PlayerAnim::PlayPlayerAnim(FString Name, int32 index)
{
	FString sectionName = FString::Printf(TEXT("%s%d"), *Name, index); //���ǳ���(�ѱ�+���� ����)
	player->PlayAnimMontage(playerMontage, 1.0f, FName(*sectionName)); //�÷��̾�� ��Ÿ�ָ� ���� �´�???
}

//[������ �ʴ� �ִϸ��̼� ---> �÷��̾� ���� ��Ÿ�� �ִϸ��̼� ��� �Լ� ����] //�ڡڡ�???
void URIM_PlayerAnim::PlayPlayerTwoAnim(FString Name, int32 index)
{
	FString sectionName = FString::Printf(TEXT("%s%d"), *Name, index); //���ǳ���(�ѱ�+���� ����)
	player->PlayAnimMontage(playerMontageTwo, 1.0f, FName(*sectionName)); //�÷��̾�� ��Ÿ�ָ� ���� �´�???
}