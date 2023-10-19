// Fill out your copyright notice in the Description page of Project Settings.


#include "MallProject/UserInterface/MainMenu.h"


//System includes:
#include "MallProject/MallProjectCharacter.h"
#include "Components/Button.h"

//Custome includes:


void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	Character = Cast<AMallProjectCharacter>(GetOwningPlayerPawn());
}
