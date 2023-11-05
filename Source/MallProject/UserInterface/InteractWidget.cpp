// Fill out your copyright notice in the Description page of Project Settings.


#include "MallProject/UserInterface/InteractWidget.h"
#include "Components/Image.h"

void UInteractWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UInteractWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInteractWidget::PlayAnimationToFade()
{
	if (FadeAnimation)
	{
		PlayAnimation(FadeAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
		//GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &UInteractWidget::TimerFunction,0.0f, true,0.f);
	}
}

void UInteractWidget::ReverseFadeAnimation()
{
	if (FadeOutAnimation)
	{
		PlayAnimation(FadeOutAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
		//if (GetAnimationCurrentTime(FadeAnimation) == 0.0f)
		//{
			//PlayAnimation(FadeAnimation, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f);
			//Timers?
		//}
	}
	//else
	//{
		//Timers?
		//PlayAnimation(FadeOutAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	//}

	//RemoveFromParent();
}

void UInteractWidget::TimerFunction()
{
}

