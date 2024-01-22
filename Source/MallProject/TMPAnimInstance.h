// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MallProject/WeaponType.h"
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
	
	UTMPAnimInstance();

	virtual void NativeInitializeAnimation() override;

	
	// This is a custome tick funtion toi update the info of the animation graph
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);
	
	//Handle turn in place variables
	void TurnInPlace();

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bWantsToRun;
	
	//Use for strafing behaviour, offset yaw
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MovementOffsetYaw;

	//Offsetyaw frame before we start moving. 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw;

	/*Rotation curve value this frama*/
	float RotationCurve;

	float RotationCurveLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bWalkieTalkie;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bHasWeaponEquipped;

	//Weapon Type for the currently equipped weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	EWeaponType EquippedWeaponType;


	//--------------------------//
	/* Root in Place animations */
	//--------------------------//
	

	//Yaw of the character this frame
	float CharacterYaw;

	float CharacterYawPreviousFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RootYawOffset;



	//================================================================================//
	// FUNCTIONS
	//================================================================================//


};
