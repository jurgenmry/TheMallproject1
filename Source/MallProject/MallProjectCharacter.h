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

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	E9_mm UMETA(DisplayName = "9mm"),
	AR UMETA(DisplayName = "Assault Rifle"),
	Shootgun UMETA(DisplayName = "Shotgun"),
	LightBatteries UMETA(DisplayName = "Bateries"),
	Other UMETA(DisplayName = "Other Ammo") // This is for doors and others
};


class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class USpotLightComponent;
class USpringArmComponent;
class UCineCameraComponent;

UCLASS(config=Game)
class AMallProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	//================================================================================//
	// Variables & Properties
	//================================================================================//

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bHasRifle;

	/* to set up the weapon of the character on overlap */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bHasWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintreadOnly, Category = "Weapon")
	bool bAiming;

	UPROPERTY(VisibleAnywhere, BlueprintreadOnly, Category = "Weapon")
	bool bWalkieTalkie;

	UPROPERTY(VisibleAnywhere, BlueprintreadOnly, Category = "Weapon")
	float CameraDefaultFOV;

	UPROPERTY(VisibleAnywhere, BlueprintreadOnly, Category = "Weapon")
	float CameraZoomFOV;

	UPROPERTY(VisibleAnywhere, BlueprintreadOnly, Category = "Weapon")
	float CurrentFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Weapon")
	float ZoomInterpSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsJogging;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bHasWeapon1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin ="0.0", ClampMax ="1.0"))
	float BaseLookUpRateX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float BaseLookUpRateY;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float BaseLookUpRateXAiming; //Turn rate while not aiming

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float BaseLookUpRateYAiming; //Look up rate when not aiming

	float CurrentRateX;
	float CurrentRateY;

	FInteractionData InteractionData;

	UPROPERTY()
	class AMallHud* HUD;




	//=======================
	//Components
	//=======================

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category= Components)
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	USkeletalMeshComponent* Feet;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	USkeletalMeshComponent* Legs;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	USkeletalMeshComponent* Torso;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	USkeletalMeshComponent* Face;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* BodySpringArm;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* LightSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* FlashLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UCineCameraComponent* FPSCameraComponent;

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

	/** Start Jogging **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JogAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ChooseWeapon1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ChooseWeapon2Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const UInputAction* Menu;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const UInputAction* AimingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* WalkieTalkieAction;


	//================================================================================//
	// FUNCTIONS
	//================================================================================//
	
	AMallProjectCharacter();

	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	
	
	///////  **** Action Functions  ****///////
	///////////////////////////////////////////

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void SetLookUpRates(float DeltaTime);

	void StartJogging();
	void EndJogging();


	void AimingButtonPressed();
	void AimingButtonReleaded();
	void CameraZoomForAiming(float DeltaTime);

	void FireWeapon();

	void TalkWalkieTalkie();
	void WalkieTalkieButtonPressed();
	void WalkieTalkieButtonReleased();
	void WalkieTalkieButtonHold();
	

	void ToggleMenu();

	void ToggleFlashLight();

	void Pause();


	//----------- Interaction code --------------/

	//For Tracing Under Crosshairs
	bool PerformTrace(FHitResult& OutHitResult, FVector& OutHitLocation);

	//This Works on the overlap Event for items
	void TraceForItems();

	//testing interaction with input button
	void InteractInputButtonPressed();

	FTimerHandle TimerHandle_Interaction;


	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();

	/* ----------------------------------------- */


	/******************* Setters *******************/
	/***********************************************/

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetHasRifle();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetHasWeapon1();

	/******************* Getters *******************/
	/***********************************************/

	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool GetHasWeapon1();

	FORCEINLINE bool GetAiming() const { return bAiming; }

	UFUNCTION()
	FORCEINLINE bool GetHasWeapon() const { return bHasWeapon; }

	UFUNCTION()
	FORCEINLINE bool GetWalkieTalkie() const { return bWalkieTalkie; }

	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UCineCameraComponent* GetFPSCameraComponent() const { return FPSCameraComponent; }

	USpotLightComponent* GetSpolight() const { return FlashLight; }

	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }


private:

	//================================================================================//
	// Variables & Properties
	//================================================================================//

	//SoundEffect based on firing weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess ="true"))
	class USoundCue* FireSound;

	//flash spawned at the barrel socket of weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* WeaponMuzzleFlash;

	//Particles on Impact ... this will needed to be changed based on what we hit.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ImpactParticle;


	//Particles from the muzzle of the weapon.. Smoketrail ... this will needed to be changed based on what we hit.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* Beanparticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* HipFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* WalkieTalkieAnimation;

	//Map to Keep Track of ammo of different weapons
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TMap<EAmmoType, int32> AmmoMap;

	/* Starting amount bullets Pistol */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int32 Starting9mmAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int32 Starting_AR_Ammo;

	//================================================================================//
	// FUNCTIONS
	//================================================================================//

	

protected:
	

	//================================================================================//
	// Variables & Properties
	//================================================================================//

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TScriptInterface<IInteractInterface> TargetInteractable;

	// Currently Equiped Weapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class AWeaponInteractableActor* EquippedWeapon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeaponInteractableActor>DefaultWeaponClass;

	//Current Weapon the character is Holding
	AWeaponInteractableActor* CurrentWeapon;


	//================================================================================//
	// FUNCTIONS
	//================================================================================//


	/* Initiliaze ammo map  with ammo values*/
	void InitializedAmmoMap();

	void SpawnDefaultWeapon(); // We will start without default weapon
	
	void CharacterHasWeapon();

	//Placing the weapon on character hand
	UFUNCTION()
	void EquipWeapon(class AWeaponInteractableActor* WeaponToEquip);//Something in parenthesis);

	// For placing the weapon on the character body
	
	void UnequipWeapon(class AWeaponInteractableActor* WeaponToEquip);


	// Beam particles 
	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation);



	

};

