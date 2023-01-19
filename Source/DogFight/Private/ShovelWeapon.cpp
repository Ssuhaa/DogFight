// Fill out your copyright notice in the Description page of Project Settings.


#include "ShovelWeapon.h"

AShovelWeapon::AShovelWeapon() //»ý¼ºÀÚ
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("StaticMesh'/Game/Geometry/SM_shovel.SM_shovel'"));
	if (TempMesh.Succeeded())
	{
		compMesh->SetStaticMesh(TempMesh.Object);
	}
	Soketname = TEXT("Shovel");
	WeaponType = EWeaponType::Shovel;
}