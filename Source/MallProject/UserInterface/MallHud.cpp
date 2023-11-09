// Fill out your copyright notice in the Description page of Project Settings.


#include "MallProject/UserInterface/MallHud.h"

//System includes:

//custome includes:
#include "MallProject/UserInterface/MainMenu.h"
#include "MallProject/UserInterface/BlurEffectWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MallProject/UserInterface/InteractWidget.h"
#include "MallProject/UserInterface/WeaponAndCrossHairDisplay.h"

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

	
	InteractionWidget = CreateWidget<UInteractWidget>(GetWorld(), InteractWidgetClass);
	if (!ensure(InteractWidgetClass != NULL)) return;
	InteractionWidget->AddToViewport();
	InteractionWidget->SetVisibility(ESlateVisibility::Visible);


	HudOverlayWidget = CreateWidget<UWeaponAndCrossHairDisplay>(GetWorld(), OverlayHudClass);
	if (!ensure(OverlayHudClass != NULL)) return;
	HudOverlayWidget->AddToViewport();
	HudOverlayWidget->SetVisibility(ESlateVisibility::Collapsed);

}


void AMallHud::DisplayMenu()
{
	
	if (MainMenuWidget)
	{
		//FString DisplayString = "DisplayMenu";
		//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, DisplayString, 1);

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
		bGamePause = false;
		UGameplayStatics::SetGamePaused(GetWorld(),false);
		
		//FInputActionBinding& toggle = InputComponent->BindAction("Pause", IE_Pressed, this, &AMallProjectCharacter::Pause);
		//toggle.bExecuteWhenPaused = true;
	}
	else
	{
		DisplayMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->bShowMouseCursor = true;
		bGamePause = false;
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		
	}
	
}

void AMallHud::ToggleOverlayWidget()
{
	if (OverlayDisplay)
	{
		HudOverlayWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HudOverlayWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMallHud::ShowInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->PlayAnimationToFade();
	}
}

void AMallHud::HideInteractionWidget() const
{ 
	if (InteractionWidget)
	{
		InteractionWidget->ReverseFadeAnimation();
	}
}

