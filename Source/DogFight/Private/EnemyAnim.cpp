// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "SH_Enemy.h"

void UEnemyAnim::AnimNotify_Attack()
{
	me = Cast<ASH_Enemy>(TryGetPawnOwner());
	me->fsm->TargetDotAttack();
}