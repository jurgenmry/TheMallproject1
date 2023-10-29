// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TMPAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MALLPROJECT_API UTMPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:

	//================================================================================//
	// Variables & Properties
	//================================================================================//



	//================================================================================//
	// FUNCTIONS
	//================================================================================//
	
	virtual void NativeInitializeAnimation() override;

	
	// This is a custome tick funtion toi update the info of the animation graph
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);


private:

	//================================================================================//
	// Variables & Properties
	//================================================================================//


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	class AMallProjectCharacter* Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsinAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsAcelerating;
	
	//================================================================================//
	// FUNCTIONS
	//================================================================================//

};
