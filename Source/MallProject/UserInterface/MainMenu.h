// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MALLPROJECT_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY()
	class AMallProjectCharacter* Character;


	//Display information on the character objects
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "PauseMenu")
	class UButton* Inventory;

	//Display the map of the game
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "PauseMenu")
	UButton* Map;

	//More options to the title screen
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "PauseMenu")
	UButton* SubmenutScreen;

	//Go back to playing the game
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "PauseMenu")
	UButton* ResumeGame;

	//The description for every button 
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "PauseMenu")
	class UTextBlock* Description;

	/*
	/* This is Part of the title menu not the pause Menu
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "PauseMenu")
	class UButton* ResumeGame;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "PauseMenu")
	UButton* LoadGame;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "PauseMenu")
	UButton* Options;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "PauseMenu")
	UButton* TitleMenu;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "PauseMenu")
	UButton* CloseGame;
	*/

protected:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
};
