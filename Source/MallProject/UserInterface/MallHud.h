// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MallHud.generated.h"

/**
 * 
 */
UCLASS()
class MALLPROJECT_API AMallHud : public AHUD
{
	GENERATED_BODY()

public:
	
	//==================================================================//
	// Properties & Variables
	//==================================================================//

	
	bool bIsMenuVisible;

	bool bGamePause = false;

	bool OverlayDisplay;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UBlurEffectWidget> BlurWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UInteractWidget> InteractWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UWeaponAndCrossHairDisplay> OverlayHudClass;


	//==================================================================//
	// Functions	
	//==================================================================//

	AMallHud();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();
	void ToggleOverlayWidget();

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;

	FORCEINLINE UInteractWidget* GetInteractionWidget() const { return InteractionWidget; }
	FORCEINLINE bool SetOverlayDisplay(bool DisplayVisible)  { return OverlayDisplay = DisplayVisible; }


protected:

	//==================================================================//
	// Properties & Variables
	//==================================================================//

	UPROPERTY()
	UMainMenu* MainMenuWidget;

	UPROPERTY()
	UBlurEffectWidget* BlurWidget;

	UPROPERTY(meta = (AllowPrivateAccess="true"))
	UInteractWidget* InteractionWidget;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	UWeaponAndCrossHairDisplay* HudOverlayWidget;

	//==================================================================//
	// Functions	
	//==================================================================//

	void BeginPlay() override;

	
};
