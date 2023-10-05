// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCTVWidget.generated.h"

/**
 * 
 */
UCLASS()
class MALLPROJECT_API UCCTVWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	class UButton* Camera1;

	UPROPERTY(meta = (BindWidget))
	UButton* Camera2;

	UPROPERTY(meta = (BindWidget))
	UButton* Camera3;

	UPROPERTY(meta = (BindWidget))
	UButton* Camera4;

	UPROPERTY(meta = (BindWidget))
	UButton* Camera5;

	
};
