// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

/**
 * 
 */
UCLASS()
class MALLPROJECT_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "Blur")
	class UImage* InteractDot;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FadeAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FadeOutAnimation;

	void PlayAnimationToFade();
	void ReverseFadeAnimation();

protected:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;


	void TimerFunction();

	FTimerHandle TimerHandle;

};
