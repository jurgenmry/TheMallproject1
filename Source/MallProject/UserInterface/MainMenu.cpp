// Fill out your copyright notice in the Description page of Project Settings.


#include "MallProject/UserInterface/MainMenu.h"


//System includes:
#include "MallProject/MallProjectCharacter.h"
#include "Components/Button.h"
#include "MallProject/UserInterface/MallHud.h"

//Custome includes:


void UMainMenu::BeginPlay()
{
	//ResumeGame->OnClicked.AddUniqueDynamic(this, &UMainMenu::OnResumeGameButtonPressed);
}
/*
bool UMainMenu:: Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false; 

	if (!ensure(ResumeGame != nullptr)) return false;
	if (ResumeGame != nullptr)
	{
		ResumeGame->OnClicked.AddUniqueDynamic(this, &UMainMenu::OnResumeGameButtonPressed);
	}
	//ResumeGame->OnClicked.AddUniqueDynamic(this, &UMainMenu::OnResumeGameButtonPressed);
	
	return bSuccess;
}
*/


void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (!ensure(ResumeGame != nullptr)) return;
	if (ResumeGame != nullptr)
	{
		ResumeGame->OnClicked.AddUniqueDynamic(this, &UMainMenu::OnResumeGameButtonPressed);
	}
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	Character = Cast<AMallProjectCharacter>(GetOwningPlayerPawn());
	HUD = Cast<AMallHud>(GetWorld()->GetFirstPlayerController()->GetHUD());

	/*if (ResumeGame)
	{
		ResumeGame->OnClicked.AddDynamic(this, &UMainMenu::OnResumeGameButtonPressed);
	}
	*/
}

void UMainMenu::OnResumeGameButtonPressed()
{
	HUD->ToggleMenu();
}
