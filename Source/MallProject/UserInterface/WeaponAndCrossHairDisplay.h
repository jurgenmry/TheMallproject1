// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponAndCrossHairDisplay.generated.h"

/**
 * 
 */
UCLASS()
class MALLPROJECT_API UWeaponAndCrossHairDisplay : public UUserWidget
{
	GENERATED_BODY()

public:

	FVector2D ScreenCenter;

	UFUNCTION()
	FVector2D SetScreenCenterCrossHair();
	
	FORCEINLINE FVector2D ReturnScreenCenter() const { return ScreenCenter; }
	

	//UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "Blur")
	//class UImage* CrossHairDot;


	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
};
