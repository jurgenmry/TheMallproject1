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

	//To Display the pause menu
	bool bIsMenuVisible;

	//UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	//TSubclassOf<class UMainMenu> MainMenuClass;

	//UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	//TSubclassOf<class UInteractionWidget> InteractionWidgetClass;

	//==================================================================//
	// Functions	
	//==================================================================//

	AMallHud();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();


	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;

protected:

	//==================================================================//
	// Properties & Variables
	//==================================================================//

	//UPROPERTY()
	//UMainMenu* MainMenuWidget;

	//UPROPERTY()
	//UInteractionWidget* InteractionWidget;

	//==================================================================//
	// Functions	
	//==================================================================//

	void BeginPlay() override;

	
};
