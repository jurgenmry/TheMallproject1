// Fill out your copyright notice in the Description page of Project Settings.


#include "MallProject/UserInterface/MallHud.h"

//System includes:

//custome includes:
#include "MallProject/UserInterface/MainMenu.h"
#include "MallProject/UserInterface/BlurEffectWidget.h"
#include "Kismet/GameplayStatics.h"

AMallHud::AMallHud()
{
}

void AMallHud::BeginPlay()
{
	Super::BeginPlay();

	MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
	if (!ensure(MainMenuClass != NULL)) return;
	MainMenuWidget->AddToViewport();
	MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);

	BlurWidget = CreateWidget<UBlurEffectWidget>(GetWorld(), BlurWidgetClass);
	if (!ensure(BlurWidgetClass != NULL)) return;
	BlurWidget->AddToViewport(-1);
	BlurWidget->SetVisibility(ESlateVisibility::Visible);

	
	//InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
	//if (!ensure(InteractionWidgetClass != NULL)) return;
	//InteractionWidget->AddToViewport(-1);
	//InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
}


void AMallHud::DisplayMenu()
{
	
	if (MainMenuWidget)
	{
		FString DisplayString = "DisplayMenu";
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, DisplayString, 1);

		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
	
}

void AMallHud::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMallHud::ToggleMenu()
{
	if (bIsMenuVisible)
	{
		HideMenu();
		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->bShowMouseCursor = false;
		UGameplayStatics::SetGamePaused(GetWorld(),false);
	}
	else
	{
		DisplayMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->bShowMouseCursor = true;
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
	
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

