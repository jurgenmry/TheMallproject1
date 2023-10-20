// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BlurEffectWidget.generated.h"

/**
 * 
 */
UCLASS()
class MALLPROJECT_API UBlurEffectWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "Blur")
	class UBackgroundBlur* Blur;

protected:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
};
