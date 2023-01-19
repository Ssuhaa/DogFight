// Fill out your copyright notice in the Description page of Project Settings.


#include "TennisWeapon.h"

ATennisWeapon::ATennisWeapon() //Ãß°¡
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT(""));
	if (tempMesh.Succeeded())
	{
		compMesh->SetStaticMesh(tempMesh.Object);
	}
}