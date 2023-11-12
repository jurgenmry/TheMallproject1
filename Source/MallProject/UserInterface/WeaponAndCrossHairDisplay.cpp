// Fill out your copyright notice in the Description page of Project Settings.


#include "MallProject/UserInterface/WeaponAndCrossHairDisplay.h"


FVector2D UWeaponAndCrossHairDisplay::SetScreenCenterCrossHair()
{
	float ScrenY = ScreenCenter.Y / 2;
	float ScrenX = ScreenCenter.X / 2;

	ScreenCenter.X = ScrenX;
	ScreenCenter.Y = ScrenY;
	
	return ScreenCenter;
}

void UWeaponAndCrossHairDisplay::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UWeaponAndCrossHairDisplay::NativeConstruct()
{
	Super::NativeConstruct();
	SetScreenCenterCrossHair();
}