// Fill out your copyright notice in the Description page of Project Settings.


#include "LollipopWeapon.h"

ALollipopWeapon::ALollipopWeapon()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("StaticMesh'/Game/Geometry/Lollipop.Lollipop'"));
	if (TempMesh.Succeeded())
	{
		compMesh->SetStaticMesh(TempMesh.Object);

	}
	compMesh->SetRelativeLocation(FVector(0, 0, -20));
	compMesh->SetRelativeRotation(FRotator(0, 0, 80));
	Soketname = TEXT("Lollipop");
}
