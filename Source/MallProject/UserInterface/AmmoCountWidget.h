// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoCountWidget.generated.h"

/**
 * 
 */
UCLASS()
class MALLPROJECT_API UAmmoCountWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "Weapon Data")
	class UImage* AmmoImage;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "Weapon Data")
	class UTextBlock* TotalAmmoCount;


	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "Weapon Data")
	class UTextBlock* CurrentWeaponAmmoCount;

protected:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	
};
