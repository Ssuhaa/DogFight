// Fill out your copyright notice in the Description page of Project Settings.


#include "TennisWeapon.h"

ATennisWeapon::ATennisWeapon() //Ãß°¡
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/Geometry/SM_shovel.SM_shovel'"));
	if (tempMesh.Succeeded())
	{
		compMesh->SetStaticMesh(tempMesh.Object);
	}
	Soketname = TEXT("Tennis");
	WeaponType = EWeaponType::Tennis;
}