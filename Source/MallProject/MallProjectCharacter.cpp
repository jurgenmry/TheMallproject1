// Copyright Epic Games, Inc. All Rights Reserved.


//System Includes
#include "MallProjectCharacter.h"
#include "MallProjectProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Runtime/CinematicCamera/Public/CineCameraComponent.h"
#include "Components/WidgetComponent.h"

//#include "CineCameraComponent.h"

//Custome Includes
#include "MallProject/UserInterface/MallHud.h"
#include "MallProject/Interactables/WeaponInteractableActor.h"

//===============================================================================+//
// Default Functions
//================================================================================//

AMallProjectCharacter::AMallProjectCharacter()
	/* Weapon States for  animation*/
	: bHasRifle(false)   // Character doesnt have a rifle at start
	, bHasWeapon(false)  //Overall Weapon Set up 
	, bAiming(false)
	, bWalkieTalkie(false)

	/* Setting for the camera while */
	, CameraDefaultFOV(0.0f)
	, CameraZoomFOV(60.0f)
	, CurrentFOV(0.0f)
	, ZoomInterpSpeed(20.0f)
	, bHasWeapon1(false) //Character does not start with any weapon

	/* Turn rates for aiming / Not aiming */
	, BaseLookUpRateX(0.5f)
	, BaseLookUpRateY(0.5f)
	, BaseLookUpRateXAiming(0.2f)
	, BaseLookUpRateYAiming(0.2f)


	/* fire weapon varaibles*/
	, bFireButtonPressed(false)
	, bShouldFire(true)
	, AutomaticFireRate(0.5)

	/* combar variables */
	,CombatState(ECombatState::ECS_Unnocupied)

	/* Weapon Amunitions */
	, Starting9mmAmmo(50)
	, Starting_AR_Ammo(120)
{
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		

	//Create the srping arm component
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->TargetArmLength = 0.0f;
	CameraBoom->SetupAttachment(GetCapsuleComponent());
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraRotationLagSpeed = 30.0f; // Was set at 10 Before
	CameraBoom->CameraLagSpeed = 40.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a CameraComponent	
	//FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	//FirstPersonCameraComponent->SetupAttachment(CameraBoom);
	//FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	//FirstPersonCameraComponent->bUsePawnControlRotation = false;

	// Create a CameraComponent	
	FPSCameraComponent = CreateDefaultSubobject<UCineCameraComponent>(TEXT("FPSCameraComponent"));
	FPSCameraComponent->SetupAttachment(CameraBoom);
	FPSCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // Position the camera
	FPSCameraComponent->bUsePawnControlRotation = false;
	

	//FirstPersonCameraComponent->bConstrainAspectRatio = true;
	//FirstPersonCameraComponent->AspectRatio = 1.333333f;


	//Create Body Spring Arm
	BodySpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("BodySpringArm"));
	BodySpringArm->TargetArmLength = 0.0f;
	//BodySpringArm->SetupAttachment(GetFirstPersonCameraComponent());
	BodySpringArm->SetupAttachment(GetFPSCameraComponent());
	BodySpringArm->bEnableCameraRotationLag = true;
	BodySpringArm->CameraRotationLagSpeed = 7.0f; 
	BodySpringArm->bUsePawnControlRotation = false;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(BodySpringArm);
	//Mesh1P->SetupAttachment(GetCapsuleComponent());
	Mesh1P->bCastDynamicShadow = true; //false
	Mesh1P->CastShadow = true; //false
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	//Create mesh for the feet
	Feet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Feet"));
	Feet->SetupAttachment(GetMesh());

	//Create mesh for the legs
	Legs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Legs->SetupAttachment(GetMesh());

	//Create mesh for the Torso
	Torso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Torso"));
	Torso->SetupAttachment(GetMesh());

	//Create mesh for the face
	Face = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	Face->SetupAttachment(GetMesh());
	Face->bRenderInMainPass = false;

	GetMesh()->bRenderInMainPass = false;

	//Create Light Spring Arm
	LightSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("LightSpringArm"));
	LightSpringArm->TargetArmLength = 0.0f;
	LightSpringArm->SetupAttachment(GetCapsuleComponent());
	LightSpringArm->SetRelativeLocation(FVector(-20.0f, 0.0f, 30.0f));
	LightSpringArm->bEnableCameraRotationLag = true;
	LightSpringArm->CameraRotationLagSpeed = 12.0f;
	LightSpringArm->bUsePawnControlRotation = true;

	//Create the Default flashlight
	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashLight"));
	FlashLight->SetupAttachment(LightSpringArm);
	FlashLight->SetVisibility(true);

	//Create The widget for the ammmo count
	AmmoWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("AmmoWidget"));
	AmmoWidget->SetupAttachment(GetMesh1P(), FName("GunRightHandSocket"));
	
}

void AMallProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	HUD = Cast<AMallHud>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (HUD == nullptr)
	{
		FString HUDString = "NULL HUD";
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, HUDString, 1);
		return;
	}

	//For the aiming Functionality
	//CameraDefaultFOV = GetFirstPersonCameraComponent()->FieldOfView;
	GetFPSCameraComponent()->SetFieldOfView(100.0f);
	CameraDefaultFOV = GetFPSCameraComponent()->FieldOfView;
	CurrentFOV = CameraDefaultFOV;

	//SpawnDefaultWeapon();

	//Initialized the value of the ammo;
	InitializedAmmoMap(); 

}

void AMallProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TraceForItems();
	CameraZoomForAiming(DeltaSeconds);
	SetLookUpRates(DeltaSeconds); //change look sensitivity base on aiming
	CalculateCrossHairSpread(DeltaSeconds); //Calculate CrossHairSpreed Multiplier
	
}

void AMallProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMallProjectCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMallProjectCharacter::Look);

		//Testing
		//Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMallProjectCharacter::BeginInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AMallProjectCharacter::EndInteract);


		//Menu Toggling
		FEnhancedInputActionEventBinding& Toggle = EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Triggered, this, &AMallProjectCharacter::Pause);
		Menu = Toggle.GetAction();//bExecuteWhenPaused = true; //EVEN THOUGH THE GAME IS PAUSED, CATCH THIS EVENT
		Menu->bTriggerWhenPaused;//bExecuteWhenPaused = true;

		//flash light Toggling
		EnhancedInputComponent->BindAction(LightAction, ETriggerEvent::Triggered, this, &AMallProjectCharacter::ToggleFlashLight);

		//Run 
		EnhancedInputComponent->BindAction(JogAction, ETriggerEvent::Triggered, this, &AMallProjectCharacter::StartJogging);
		EnhancedInputComponent->BindAction(JogAction, ETriggerEvent::Completed, this, &AMallProjectCharacter::EndJogging);

		//Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMallProjectCharacter::FireButtonPressed);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AMallProjectCharacter::FireButtonReleased);

		//Reloading
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AMallProjectCharacter::ReloadButtonPressed);


		//adding Weapons 1 to 4 
		EnhancedInputComponent->BindAction(ChooseWeapon1Action, ETriggerEvent::Triggered, this, &AMallProjectCharacter::OneKeyPressed); 
		EnhancedInputComponent->BindAction(ChooseWeapon2Action, ETriggerEvent::Triggered, this, &AMallProjectCharacter::TwoKeyPressed);
		EnhancedInputComponent->BindAction(ChooseWeapon2Action, ETriggerEvent::Triggered, this, &AMallProjectCharacter::ThreeKeyPressed);
		EnhancedInputComponent->BindAction(ChooseWeapon2Action, ETriggerEvent::Triggered, this, &AMallProjectCharacter::FourKeyPressed);


		//Aiming
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Triggered, this, &AMallProjectCharacter::AimingButtonPressed);
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Completed, this, &AMallProjectCharacter::AimingButtonReleaded);

		//WalkieTalkie 
		EnhancedInputComponent->BindAction(WalkieTalkieAction, ETriggerEvent::Triggered, this, &AMallProjectCharacter::WalkieTalkieButtonPressed);
		//EnhancedInputComponent->BindAction(WalkieTalkieAction, ETriggerEvent::Ongoing, this, &AMallProjectCharacter::WalkieTalkieButtonHold);
		EnhancedInputComponent->BindAction(WalkieTalkieAction, ETriggerEvent::Completed, this, &AMallProjectCharacter::WalkieTalkieButtonReleased);
	}
}

void AMallProjectCharacter::CalculateCrossHairSpread(float DeltaTime)
{
	//create a vector with the velocity of the character
	FVector2D WalkSpeedRange{ 0.0f, 200.0f };
	//Define a range from 0 to 1
	FVector2D VelocityMultiplierRange{ 0.0f, 1.0f };
	FVector Velocity{ GetVelocity() };
	Velocity.Z = 0.0f;

	CrossHairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

	CrossHairSpreedMultiplier = 0.5f + CrossHairVelocityFactor;
}

void AMallProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMallProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		//AddControllerYawInput(LookAxisVector.X * BaseTurnRate * GetWorld()->DeltaTimeSeconds);
		//AddControllerPitchInput(LookAxisVector.Y * BaseLookUpRate * GetWorld()->DeltaTimeSeconds);
		
		AddControllerYawInput(LookAxisVector.X * CurrentRateX); //Original
		AddControllerPitchInput(LookAxisVector.Y * CurrentRateY);
	}
}

void AMallProjectCharacter::FireButtonPressed()
{
	bFireButtonPressed = true;

	FireWeapon();
}

void AMallProjectCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
}

void AMallProjectCharacter::StartFireTimer()
{
	CombatState = ECombatState::ECS_FireTimerInProgress;

	GetWorldTimerManager().SetTimer(
		AutoFireTimer, this, &AMallProjectCharacter::AutoFireReset,
		AutomaticFireRate);
}

void AMallProjectCharacter::AutoFireReset()
{
	CombatState = ECombatState::ECS_Unnocupied;
	if (WeaponHasAmmo())
	{
		if (bFireButtonPressed)
		{
			FireWeapon();
		}

		/*
		if(!CarryingAmmo() && !bFireButtonPressed)
		{
			//Reload The weapon
			ReloadWeapon();
		}
		*/
	}
}

void AMallProjectCharacter::SetLookUpRates(float DeltaTime)
{
	
	if (bAiming)
	{
		CurrentRateX = BaseLookUpRateXAiming;
		CurrentRateY = BaseLookUpRateYAiming;
	}
	else
	{
		CurrentRateX = BaseLookUpRateX;
		CurrentRateY = BaseLookUpRateY;
	}
}



//================================================================================//
// Custome Functions
//================================================================================//

// INPUNT FUNCTIONS 

void AMallProjectCharacter::InteractInputButtonPressed()
{
	InteractionData.bShouldTraceForItems = true;
}

void AMallProjectCharacter::Pause()
{
	//Menu->bTriggerWhenPaused;
	UInputAction* Menu2 = const_cast<UInputAction*>(Menu);
	Menu2->bTriggerWhenPaused = true;
	
	if (HUD -> bGamePause)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		HUD->ToggleMenu();
	}
	else
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		HUD->ToggleMenu();
	}
}

void AMallProjectCharacter::ToggleMenu()
{

	HUD->ToggleMenu();
}

void AMallProjectCharacter::ToggleFlashLight()
{
	FlashLight->ToggleVisibility();
}

void AMallProjectCharacter::StartJogging()
{
	//FString Sprint = "Is Sprinting";
	//GEngine->AddOnScreenDebugMessage(5, 3.0f, FColor::Red, Sprint, 1);
	this->GetCharacterMovement()->MaxWalkSpeed = 630.0f;
	bIsJogging = true;
}

void AMallProjectCharacter::EndJogging()
{
	//FString Sprint = "Is NOT Sprinting";
	//GEngine->AddOnScreenDebugMessage(5, 3.0f, FColor::Red, Sprint, 1);
	
	this->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	bIsJogging = false;
}

void AMallProjectCharacter::AimingButtonPressed()
{
	if (bHasWeapon)
	{
		bAiming = true;
	}
}

void AMallProjectCharacter::AimingButtonReleaded()
{
	bAiming = false;
}

void AMallProjectCharacter::CameraZoomForAiming(float DeltaTime)
{

	if (bAiming)
	{
		CurrentFOV = FMath::FInterpTo(
			CurrentFOV, CameraZoomFOV, DeltaTime, ZoomInterpSpeed);
		//GetFirstPersonCameraComponent() ->PostProcessSettings.DepthOfFieldFstop  
		//GetFirstPersonCameraComponent()->PostProcessSettings.DepthOfFieldFocalDistance = 50.0f;
		//GetFirstPersonCameraComponent()->PostProcessSettings.DepthOfFieldFstop = 11.0f;
		//GetFirstPersonCameraComponent()->PostProcessSettings.DepthOfFieldMinFstop = 1.0f;
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(
			CurrentFOV, CameraDefaultFOV, DeltaTime, ZoomInterpSpeed);
		//GetFirstPersonCameraComponent()->PostProcessSettings.DepthOfFieldFocalDistance = 0.0f;
		//GetFirstPersonCameraComponent()->PostProcessSettings.DepthOfFieldFstop = 4.0f;
		//GetFirstPersonCameraComponent()->PostProcessSettings.DepthOfFieldMinFstop = 1.2f;
	}

	//GetFirstPersonCameraComponent()->SetFieldOfView(CurrentFOV);
	GetFPSCameraComponent()->SetFieldOfView(CurrentFOV);
}


///  This code has to do with firing the wapon and do line traces. 
/// 


// This Trace will allow us to trace for all items that have a interface
void AMallProjectCharacter::TraceForItems()
{
	if (InteractionData.bShouldTraceForItems)
	{
		FVector HitLocation; //Maybe this need to be adjusted
		FHitResult ItemTraceresult;
		PerformTrace(ItemTraceresult, HitLocation);
		if (ItemTraceresult.bBlockingHit)
		{

			//InteractionData.CurrentIntertactable = Cast<AActor>(ItemTraceresult.GetActor());
			//FoundInteractable(InteractionData.CurrentIntertactable);

			//if the class implements the interact interface check item
			if (ItemTraceresult.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
			{
				//HUD->ShowInteractionWidget();
				
				//If they are not the same we found new Interactable
				if (ItemTraceresult.GetActor() != InteractionData.CurrentIntertactable)
				{
					FoundInteractable(ItemTraceresult.GetActor());
					return;
				}

				//if its the same we dont need to do anything
				if (ItemTraceresult.GetActor() == InteractionData.CurrentIntertactable)
				{
					return;
				}
			}
			//HUD->HideInteractionWidget();
		}
	}

	//HUD->HideInteractionWidget();
	NoInteractableFound();
}

bool AMallProjectCharacter::PerformTrace(FHitResult& OutHitResult, FVector& OutHitLocation)
{
	FVector2D VierportSize;

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(VierportSize);
	}

	//Divides the screen in  half to set crosshairs
	FVector2D CrossHairWorldLocation{ VierportSize.X / 2.0f, VierportSize.Y / 2.0f };
	FVector CrossHairWorldDirection;
	FVector CrossHairWorldPosition;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrossHairWorldLocation,
		CrossHairWorldPosition,
		CrossHairWorldDirection);

	if (bScreenToWorld)
	{
		//Trace from center of the Screen outwards
		const FVector Start{ CrossHairWorldPosition };
		const FVector End{ Start + CrossHairWorldDirection * 50'000.f };
		OutHitLocation = End;

		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Visibility);

		if (OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}
	return false;
}

bool AMallProjectCharacter::GetBeamEndLocation(
	const FVector& MuzzleSocketLocation,
	FVector& OutBeamLocation)
{

	//Check for croshairTraceHit;
	FHitResult CrosshairHitResult;
	bool bCrossHairHit = PerformTrace(CrosshairHitResult, OutBeamLocation);

	if (bCrossHairHit)
	{
		// Tentative beam location - Still need to check  from the gun 
		OutBeamLocation = CrosshairHitResult.Location;
	}
	else // No crosshair hit
	{
		//OutBeamLocation is the end location for the line trace
	}

	//Perform a second trace, this time for the gun barrel 
	FHitResult WeaponTraceHit;
	const FVector WeaponTraceStart{ MuzzleSocketLocation };
	const FVector StartToEnd{ OutBeamLocation - MuzzleSocketLocation };
	const FVector WeaponTraceEnd{ MuzzleSocketLocation + StartToEnd * 1.25f };

	GetWorld()->LineTraceSingleByChannel(
		WeaponTraceHit,
		WeaponTraceStart,
		WeaponTraceEnd,
		ECollisionChannel::ECC_Visibility);

	if (WeaponTraceHit.bBlockingHit) //Object Bewtween the barrel and the beam end point
	{
		OutBeamLocation = WeaponTraceHit.Location;
		return true;
	}
	return false;
}


//Actual job of firing the weapon and its effects
void AMallProjectCharacter::FireWeapon()
{
	if (EquippedWeapon == nullptr) return;
	
	if (CombatState != ECombatState::ECS_Unnocupied) return;

	if (WeaponHasAmmo())
	{
		PlayFireSound();     //PlayFire Sound
		SendBullet();        //Send bullet
		PlayFireMontage();   //Play Hit Fire Montage
		EquippedWeapon->DecrementAmmo(); //Substract one from the weapon ammo;
		StartFireTimer();
	}
}

void AMallProjectCharacter::PlayFireSound()
{
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this, FireSound);
	}
}

void AMallProjectCharacter::SendBullet()
{
	//So far will not work because the socket should be in the weapon not in the character.
	const USkeletalMeshSocket* BarrelSocket = EquippedWeapon->GetItemSkeleton()->GetSocketByName(TEXT("BarrelSocket"));

	if (BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(EquippedWeapon->GetItemSkeleton());
		if (WeaponMuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponMuzzleFlash, SocketTransform);
		}

		FVector BeamEnd;
		bool bBeamEnd = GetBeamEndLocation(SocketTransform.GetLocation(), BeamEnd);

		if (bBeamEnd)
		{
			if (ImpactParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ImpactParticle,
					BeamEnd);
			}

			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				Beanparticles,
				SocketTransform);
			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
		}
	}
}

void AMallProjectCharacter::PlayFireMontage()
{
	//Play Hit Fire Montage
	UAnimInstance* AnimIntance = GetMesh1P()->GetAnimInstance();
	if (AnimIntance && HipFire)
	{
		AnimIntance->Montage_Play(HipFire);
		AnimIntance->Montage_JumpToSection(FName("Fire"));
	}
}


// This code has to do with the walkie talkie 

void AMallProjectCharacter::TalkWalkieTalkie()
{
	/*
	UAnimInstance* AnimIntance = GetMesh1P()->GetAnimInstance();
	if (AnimIntance && WalkieTalkieAnimation)
	{
		if (bWalkieTalkie)
		{

			AnimIntance->Montage_Play(WalkieTalkieAnimation);
			AnimIntance->Montage_JumpToSection(FName("WalkieDown"));
			//AnimIntance->Montage_JumpToSection(FName("W"));
		}
		/*else
		{
			AnimIntance->Montage_Play(WalkieTalkieAnimation);
			AnimIntance->Montage_JumpToSection(FName("WalkieDown"));
		}
		
	}
	*/


}

void AMallProjectCharacter::WalkieTalkieButtonPressed()
{
	bWalkieTalkie = true;
	//TalkWalkieTalkie();
}

void AMallProjectCharacter::WalkieTalkieButtonReleased()
{
	bWalkieTalkie = false;
	//TalkWalkieTalkie();
}

void AMallProjectCharacter::WalkieTalkieButtonHold()
{
	UAnimInstance* AnimIntance = GetMesh1P()->GetAnimInstance();
	if (AnimIntance && WalkieTalkieAnimation)
	{
		AnimIntance->Montage_Play(WalkieTalkieAnimation);
		AnimIntance->Montage_JumpToSection(FName("WalkieHold"));
	}
}



// code for choosing of weapons
void AMallProjectCharacter::OneKeyPressed()
{
	if (!EquippedWeapon) return;
	if (EquippedWeapon->GetSlothIndex() == 0) return;
	ExchangeInventoryItems(EquippedWeapon->GetSlothIndex(), 0);
}

void AMallProjectCharacter::TwoKeyPressed()
{
	if (!EquippedWeapon) return;
	if (EquippedWeapon->GetSlothIndex() == 1) return;
	ExchangeInventoryItems(EquippedWeapon->GetSlothIndex(), 1);
}

void AMallProjectCharacter::ThreeKeyPressed()
{
	if (!EquippedWeapon) return;
	if (EquippedWeapon->GetSlothIndex() == 2) return;
	ExchangeInventoryItems(EquippedWeapon->GetSlothIndex(), 2);
}

void AMallProjectCharacter::FourKeyPressed()
{
	if (!EquippedWeapon) return;
	if (EquippedWeapon->GetSlothIndex() == 3) return;
	ExchangeInventoryItems(EquippedWeapon->GetSlothIndex(), 4);
}

void AMallProjectCharacter::ExchangeInventoryItems(int32 CurrentItemIndex, int32 NewItemIndex)
{
	if (EquippedWeapon == nullptr) return;
	if ((CurrentItemIndex == NewItemIndex) && (NewItemIndex >= Inventory.Num())) return;

	AWeaponInteractableActor* OldEquippedWeapon = EquippedWeapon;
	AWeaponInteractableActor* NewWeapon = Cast<AWeaponInteractableActor>(Inventory[NewItemIndex]);

	EquipWeapon(NewWeapon);
	OldEquippedWeapon->SetItemState(EItemState::Item_PickedUp);
	NewWeapon->SetItemState(EItemState::Equipped);
}


// This code is for reloading

 void AMallProjectCharacter::ReloadWeapon()
{
	if (CombatState != ECombatState::ECS_Unnocupied) return;
	if (EquippedWeapon == nullptr) return;

	if (CarryingAmmo())
	{
		CombatState = ECombatState::ECS_Reloading;
		UAnimInstance* AnimInstance = GetMesh1P()->GetAnimInstance();
		if (AnimInstance && ReloadMontage)
		{
			AnimInstance->Montage_Play(ReloadMontage);
			AnimInstance->Montage_JumpToSection(
				EquippedWeapon->GetReloadMontageSection());
		}
	}
}
void AMallProjectCharacter::FinishReloading()
{
	CombatState = ECombatState::ECS_Unnocupied;

	if (EquippedWeapon == nullptr) return;

	//Update the ammo Map
	const auto AmmoType{ EquippedWeapon->GetAmmoType() };

	if (AmmoMap.Contains(AmmoType))
	{
		//Ammount of ammo the character is carrying of the equipped weapon Type
		int32 CarriedAmmo = AmmoMap[AmmoType];

		//Space left in the magazine of Equipped weapon;
		const int32 MagEmptySpace = EquippedWeapon->GetMagazineCapacity() 
			- EquippedWeapon->GetAmmoCount();

		//Reload the magazine with all the ammo we are carrying
		if (MagEmptySpace > CarriedAmmo)
		{
			EquippedWeapon->ReloadAmmo(CarriedAmmo);
			CarriedAmmo = 0; 
			AmmoMap.Add(AmmoType, CarriedAmmo);
		}

		// fill in the magazine 
		else
		{
			EquippedWeapon->ReloadAmmo(MagEmptySpace);
			CarriedAmmo -= MagEmptySpace;
			AmmoMap.Add(AmmoType, CarriedAmmo);
		}
	}
}
void AMallProjectCharacter::ReloadButtonPressed()
{
	ReloadWeapon();
}
bool AMallProjectCharacter::CarryingAmmo()
{
	if (EquippedWeapon == nullptr) return false; 

	EAmmoType AmmoType = EquippedWeapon->GetAmmoType();

	if (AmmoMap.Contains(AmmoType))
	{
		return AmmoMap[AmmoType] > 0;
	}
	return false;
}


/// 
///  This code has to do with Interactables and all the logic under it  
///  we will review this description and code in later intance
/// 

void AMallProjectCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (InteractionData.bIsInteracting == true)
	{
		//UE_LOG(LogTemp, Warning, TEXT("InteractionData.bIsInteracting == true"));
		//HUD->HideInteractionWidget();
		EndInteract();
	}

	if (InteractionData.CurrentIntertactable)
	{
		TargetInteractable = InteractionData.CurrentIntertactable;
		TargetInteractable->EndFocus();
		HUD->HideInteractionWidget();
	}

	InteractionData.CurrentIntertactable = NewInteractable;
	TargetInteractable = NewInteractable;

	InteractionData.bIsInteracting = true; // check this line of code. Might be needed to remove it 

	//HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus(); //What happend after beggins focus? Where are we getting redirected ?
	HUD->ShowInteractionWidget();
}

void AMallProjectCharacter::NoInteractableFound()
{
	//HUD->HideInteractionWidget();

	if (InteractionData.bIsInteracting == true)
	{
		InteractionData.bIsInteracting = false;
	}

	if (InteractionData.CurrentIntertactable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
			HUD->HideInteractionWidget();
		}

		InteractionData.CurrentIntertactable = nullptr;
		TargetInteractable = nullptr;
		HUD->HideInteractionWidget();
	}
}

void AMallProjectCharacter::BeginInteract()
{
	//UE_LOG(LogTemp, Warning, TEXT("Calling begin interact Override on interface test actor"));

	if (InteractionData.CurrentIntertactable)
	{
		//FString Banana = TEXT("Begin Interact");
		//GEngine->AddOnScreenDebugMessage(5, 5.0f, FColor::Red, Banana, 1);

		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginFocus(); //?
			HUD->ShowInteractionWidget();
			

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				//GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
				//	this,
					//&AMallProjectCharacter::Interact,
					//TargetInteractable->InteractableData.InteractionDuration,
					//false);
			}
		}

		HUD->HideInteractionWidget();
	}
}

void AMallProjectCharacter::EndInteract()
{
	FString Banana = TEXT("End Interact");
	GEngine->AddOnScreenDebugMessage(7, 5.0f, FColor::Red, Banana, 1);

	InteractionData.bIsInteracting = false;
	

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void AMallProjectCharacter::Interact()
{
	InteractionData.bIsInteracting = false;

	if (IsValid(TargetInteractable.GetObject()))
	{
		
		//I think this part of code should go when begin interact
		if (TargetInteractable->InteractableData.InteractableType == EInteractableType::Weapon)
		{
			EquippedWeapon = Cast<AWeaponInteractableActor>(TargetInteractable.GetObject());
			
			if (EquippedWeapon->GetPickUpSound())
			{
				UGameplayStatics::PlaySound2D(this, EquippedWeapon->GetPickUpSound());
			}

			if (EquippedWeapon)
			{
				if (Inventory.Num() < InventoryCapacity)
				{
					
					Inventory.Add(EquippedWeapon);
					EquippedWeapon->SeSlothIndex(Inventory.Num());
					EquippedWeapon->SetItemState(EItemState::Item_PickedUp);
					//EquipWeapon(EquippedWeapon);
					HUD->HideInteractionWidget();
					EquippedWeapon->BeginInteract();
				}
				else
				{
					//Inventory is full 
					
				}

			}
			else
			{
				AWeaponInteractableActor* Weapon = NewObject<AWeaponInteractableActor>(TargetInteractable.GetObject());//<AWeaponInteractableActor*>(TargetInteractable);
				HUD->HideInteractionWidget();
				Weapon->BeginInteract();
				EquippedWeapon->SetActorEnableCollision(false);
				EquippedWeapon->GetItemSkeleton()->SetVisibility(false);
			}
		}

		else
		{
			TargetInteractable->Interact(this); // Here we should handle the special information for actors
		}
	}
}


/// 
///  This code has to do with  Equip and unequip weapons and all the logic under it  
///  we will review this description and code in later intance
/// 


void AMallProjectCharacter::SpawnDefaultWeapon()
{
	if (DefaultWeaponClass)
	{
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponInteractableActor>(DefaultWeaponClass);
		const USkeletalMeshSocket* RHandSocket = GetMesh1P()->GetSocketByName(FName("GunRightHandSocket"));

		if (RHandSocket)
		{
			RHandSocket->AttachActor(CurrentWeapon, GetMesh1P());
			CurrentWeapon->SetItemState(EItemState::Equipped);
		}
	}
}

void AMallProjectCharacter::CharacterHasWeapon() //Need to check when to call
{
	if (bHasWeapon)
	{
		HUD->SetOverlayDisplay(true);
	}
}

void AMallProjectCharacter::EquipWeapon(AWeaponInteractableActor* WeaponToEquip)
{
	if(WeaponToEquip)
	{
		//WeaponToEquip->SetItemState(EItemState::Equipped);

		//WeaponToEquip->GetBoxComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		//WeaponToEquip->GetSphereComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		//WeaponToEquip->GetItemSkeleton()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		

		//Create USkeletal Socket to attach the weapon
		const USkeletalMeshSocket* RHandSocket = GetMesh1P()->GetSocketByName(FName("GunRightHandSocket"));
		if (RHandSocket)
		{
			RHandSocket->AttachActor(WeaponToEquip, GetMesh1P());

			//WeaponToEquip->AttachToComponent(GetMesh1P(), FAttachmentTransformRules::KeepRelativeTransform, FName("GunRightHandSocket"));

			//FString AddWeapon = "AddWeapon to Hand";
			//GEngine->AddOnScreenDebugMessage(10, 5.0f, FColor::Red, AddWeapon, 0);

			//WeaponToEquip->GetBoxComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			//WeaponToEquip->GetSphereComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			//WeaponToEquip->GetItemSkeleton()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			//WeaponToEquip->GetItemSkeleton()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			CurrentWeapon = WeaponToEquip;
			CurrentWeapon->SetItemState(EItemState::Equipped);
			//CharacterHasWeapon();
			
			//WeaponToEquip = CurrentWeapon;

		}
	}

	//DefaultWeapon = Something in parenthesis 
}



/// 
///  This code has to do with  Weapons Ammo
///  we will review this description and code in later intance
///

void AMallProjectCharacter::InitializedAmmoMap()
{
	AmmoMap.Add(EAmmoType::E9_mm, Starting9mmAmmo);
	AmmoMap.Add(EAmmoType::AR, Starting_AR_Ammo);
}


bool AMallProjectCharacter::WeaponHasAmmo()
{
	if (EquippedWeapon == nullptr)
	{
		return false;
	}
	
	return EquippedWeapon->GetAmmoCount() > 0;
}