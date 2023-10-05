// Fill out your copyright notice in the Description page of Project Settings.


#include "MallProject/UserInterface/MallHud.h"

AMallHud::AMallHud()
{
}

void AMallHud::BeginPlay()
{
	Super::BeginPlay();

	//MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
	//if (!ensure(MainMenuClass != NULL)) return;
	//MainMenuWidget->AddToViewport(5);
	//MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);

	//InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
	//if (!ensure(InteractionWidgetClass != NULL)) return;
	//InteractionWidget->AddToViewport(-1);
	//InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
}


void AMallHud::DisplayMenu()
{/*
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
	*/
}

void AMallHud::HideMenu()
{/*
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	*/
}

void AMallHud::ToggleMenu()
{/*
	if (bIsMenuVisible)
	{
		HideMenu();
		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->bShowMouseCursor = false;
	}
	else
	{
		DisplayMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->bShowMouseCursor = true;
	}
	*/
}

void AMallHud::ShowInteractionWidget() const
{/*
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	*/
}

void AMallHud::HideInteractionWidget() const
{ /*
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	*/
}

