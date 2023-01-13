// Fill out your copyright notice in the Description page of Project Settings.


#include "GunWeapon.h"

AGunWeapon::AGunWeapon()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("StaticMesh'/Game/Geometry/Gun.Gun'"));
	if(TempMesh.Succeeded())
	{
		compMesh->SetStaticMesh(TempMesh.Object);
	}

	Soketname = TEXT("Gun");
	WeaponType = EWeaponType::Gun;
}