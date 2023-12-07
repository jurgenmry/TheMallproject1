// Fill out your copyright notice in the Description page of Project Settings.


#include "MallProject/UserInterface/MallHud.h"

//System includes:
#include "Engine/Texture.h"

//custome includes:
#include "MallProject/UserInterface/MainMenu.h"
#include "MallProject/UserInterface/BlurEffectWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MallProject/UserInterface/InteractWidget.h"
#include "MallProject/UserInterface/WeaponAndCrossHairDisplay.h"
#include "MallProject/UserInterface/WeaponAndCrossHairDisplay.h"
#include "MallProject/MallProjectCharacter.h"

//For testing
#include "Blueprint/UserWidget.h"


AMallHud::AMallHud(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void AMallHud::BeginPlay()
{
	Super::BeginPlay();

	//CrossHairDraw = true;

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
	HudOverlayWidget->SetVisibility(ESlateVisibility::Visible);

	//MainCharacter = Cast<AMallProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//if (!ensure(MainCharacter != NULL)) return;

	//test Place 
	//this->DrawHUD();
	//DrawCrossHairs(true);


	//For testing the AMMO
	//AmmoClassWidget = CreateWidget<UUserWidget>(GetWorld(), AmmoClass);
	//if (!ensure(AmmoClass != NULL)) return;
	//AmmoClassWidget->AddToViewport();
	//AmmoClassWidget->SetVisibility(ESlateVisibility::Visible);
	
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

/*
void AMallHud::DrawCrossHairs(bool CrossHairVisible)
{
	if (CrossHairVisible)
	{
		if (!ensure(CrossHairTexture != NULL)) return;
		this->DrawTexture(
			CrossHairTexture, 
			HudOverlayWidget->ReturnScreenCenter().X, 
			HudOverlayWidget->ReturnScreenCenter().Y, 
			64.0f, 64.0f,0.0f, 0.0f, 1.0f, 1.0f);
	}
}

void AMallHud::DrawHUD()
{
	Super::DrawHUD();

	if (CrossHairTexture)
	{
		MainCharacter = Cast<AMallProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
		if (MainCharacter->bHasWeapon1)
		{
			//Find the center of the canvas
			FVector2D Canvascenter{ Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f };

			//Offset the center of the centure to align with the canvas
			FVector2D CrossHairDrawPositionCenter(Canvascenter.X - (CrossHairTexture->GetSurfaceWidth() * 0.5),
				Canvascenter.Y - (CrossHairTexture->GetSurfaceWidth() * 0.5));

			//Draw the crossHairs at the center point
			FCanvasTileItem TileItem(CrossHairDrawPositionCenter, CrossHairTexture->Resource, FLinearColor::White);
			TileItem.BlendMode = SE_BLEND_Translucent;
			Canvas->DrawItem(TileItem);
		}

	}
}
*/
/*
void AMallHud::DrawHUD()
{
	
	//ReceiveDrawHUD()
	DrawCrossHairs(true);
}
*/

