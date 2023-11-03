// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "MallProject/Interfaces/InteractInterface.h"
#include "InputActionValue.h"
#include "MallProjectCharacter.generated.h"


USTRUCT()
struct FInteractionData
{
	//This might change for sphere collision
	GENERATED_USTRUCT_BODY()

		FInteractionData()
		: CurrentIntertactable(nullptr)
		, LastInteractionCheckedTime(0.0f)
		, bShouldTraceForItems(false)
		, bIsInteracting(false)
	{

	};

	UPROPERTY()
		AActor* CurrentIntertactable;

	UPROPERTY()
	float LastInteractionCheckedTime;

	bool bShouldTraceForItems;

	bool bIsInteracting;
};



class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class USpotLightComponent;
class USpringArmComponent;

UCLASS(config=Game)
class AMallProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	//================================================================================//
	// Variables & Properties
	//================================================================================//

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsJogging;

	FInteractionData InteractionData;

	//=======================
	//Components
	//=======================

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category= Components)
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* FlashLight;


	//=======================
	//Input
	//=======================

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Interaction With the world action **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/** Call the Pause menu action **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MenuAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Turn off On Flash light **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JogAction;


	//================================================================================//
	// FUNCTIONS
	//================================================================================//
	
	AMallProjectCharacter();

	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);


	/******************* Setters *******************/
	/***********************************************/

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/******************* Getters *******************/
	/***********************************************/

	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	USpotLightComponent* GetSpolight() const { return FlashLight; }

	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

protected:
	

	//================================================================================//
	// Variables & Properties
	//================================================================================//

	UPROPERTY()
	class AMallHud* HUD;
	
	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TScriptInterface<IInteractInterface> TargetInteractable;



	//================================================================================//
	// FUNCTIONS
	//================================================================================//


	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void ToggleMenu();

	void ToggleFlashLight();

	void StartJogging();
	void EndJogging();


public:

	//----------- Interaction code --------------/

	//For Tracing Under Crosshairs
	bool PerformTrace(FHitResult& OutHitResult);

	//This Works on the overlap Event for items
	void TraceForItems();

	//testing interaction with input button
	void InteractInputButtonPressed();

	FTimerHandle TimerHandle_Interaction;

	//input
	//void TraceButtonPressed()

	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();

	void Pause();

	const UInputAction* Menu;
};

