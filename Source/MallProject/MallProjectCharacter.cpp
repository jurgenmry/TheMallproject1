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

//Custome Includes
#include "MallProject/UserInterface/MallHud.h"
#include "MallProject/Interactables/WeaponInteractableActor.h"

//////////////////////////////////////////////////////////////////////////
// AMallProjectCharacter

AMallProjectCharacter::AMallProjectCharacter()
	
{
	//Overall Weapon Set up
	bHasWeapon = false;

	// Character doesnt have a rifle at start
	bHasRifle = false;

	//Character does not start with any weapon
	bHasWeapon1 = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		

	//Create the srping arm component
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->TargetArmLength = 0.0f;
	CameraBoom->SetupAttachment(GetCapsuleComponent());
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraRotationLagSpeed = 10.0f;
	CameraBoom->bUsePawnControlRotation = false;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(CameraBoom);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	//FirstPersonCameraComponent->bConstrainAspectRatio = true;
	//FirstPersonCameraComponent->AspectRatio = 1.333333f;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	//Create the Default flashlight
	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashLight"));
	FlashLight->SetupAttachment(FirstPersonCameraComponent);
	FlashLight->SetVisibility(true);
	
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

	//SpawnDefaultWeapon();

}

void AMallProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TraceForItems();
}

//////////////////////////////////////////////////////////////////////////// Input



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
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMallProjectCharacter::FireWeapon);

		//adding weapon1
		EnhancedInputComponent->BindAction(ChooseWeapon1Action, ETriggerEvent::Triggered, this, &AMallProjectCharacter::SetHasWeapon1);

		//adding weapon1
		EnhancedInputComponent->BindAction(ChooseWeapon2Action, ETriggerEvent::Triggered, this, &AMallProjectCharacter::SetHasRifle);

	}
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
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMallProjectCharacter::SetHasRifle()
{
	if (CurrentWeapon)
	{
		if (!bHasRifle)
		{
			//Play Animation Montage
			bHasRifle = true;
		}

		else if (bHasWeapon1)
		{
			//Play Animation Montage
			bHasRifle = false;
		}
	}
}

void AMallProjectCharacter::SetHasWeapon1()
{
	if (CurrentWeapon && CurrentWeapon ->WeaponType == EWeaponType::Pistol)
	{
		if (!bHasWeapon1)
		{
			//Play Animation Montage
			CurrentWeapon->GetItemSkeleton()->SetVisibility(true);
			bHasWeapon1 = true;
		}

		else if (bHasWeapon1)
		{
			//Play Animation Montage
			CurrentWeapon->GetItemSkeleton()->SetVisibility(false);
			bHasWeapon1 = false;
		}
	}
}

bool AMallProjectCharacter::GetHasRifle()
{
	return bHasRifle;
}

bool AMallProjectCharacter::GetHasWeapon1()
{
	return false;
}



//================================================================================//
// Custome Functions
//================================================================================//

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

void AMallProjectCharacter::FireWeapon()
{
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this, FireSound);
	}

	//So far will not work because the socket should be in the weapon not in the character.
	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName(TEXT("BarrelSocket"));

	if (BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());
		if (WeaponMuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponMuzzleFlash, SocketTransform);
		}

		FVector BeamEnd;
		bool bBeamEnd = GetBeamEndLocation(SocketTransform.GetLocation(),BeamEnd);
		
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
	UAnimInstance* AnimIntance = GetMesh1P()->GetAnimInstance();
	if (AnimIntance && HipFire)
	{
		AnimIntance->Montage_Play(HipFire);
		AnimIntance->Montage_JumpToSection(FName("Fire"));
	}
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
		FString Banana = TEXT("Begin Interact");
		GEngine->AddOnScreenDebugMessage(5, 5.0f, FColor::Red, Banana, 1);

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
		TargetInteractable->Interact(this);


		//I think this part of code should go when begin interact
		if (TargetInteractable->InteractableData.InteractableType == EInteractableType::Weapon)
		{
			EquippedWeapon = Cast<AWeaponInteractableActor>(TargetInteractable.GetObject());
			if (EquippedWeapon)
			{
				EquipWeapon(EquippedWeapon);
				HUD->HideInteractionWidget();
				EquippedWeapon->BeginInteract();
			}
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

void AMallProjectCharacter::EquipWeapon(AWeaponInteractableActor* WeaponToEquip)
{

	if(WeaponToEquip)
	{
		FString AddWeapon = "AddWeapon to Hand";
		GEngine->AddOnScreenDebugMessage(10, 5.0f, FColor::Red, AddWeapon, 0);

		//WeaponToEquip->SetItemState(EItemState::Equipped);

		//Create USkeletal Socket to attach the weapon
		const USkeletalMeshSocket* RHandSocket = GetMesh1P()->GetSocketByName(FName("GunRightHandSocket"));
		if (RHandSocket)
		{
			RHandSocket->AttachActor(WeaponToEquip, GetMesh1P());

			//WeaponToEquip->GetBoxComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			//WeaponToEquip->GetSphereComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			//WeaponToEquip->GetItemSkeleton()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			//WeaponToEquip->GetItemSkeleton()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			WeaponToEquip->SetItemState(EItemState::Equipped);

			bHasWeapon = true;
			CurrentWeapon = WeaponToEquip;

			
		}
	}

	//DefaultWeapon = Something in parenthesis 
}

//Based on the type of weapon, attach the weapon to the corresponding socket
void AMallProjectCharacter::UnequipWeapon(AWeaponInteractableActor* WeaponToEquip)
{
	/*
	* 
	* ////// This was supposed to be code to attach to different sockets . but took other aproach
	* ////// of hidding the weapon mesh.
	* 
	* 
	//Create USkeletal Socket to attach the weapon
	const USkeletalMeshSocket* HipLeftSocket = GetMesh1P()->GetSocketByName(FName("HipLSocket"));
	const USkeletalMeshSocket* ClaviculeSocket = GetMesh1P()->GetSocketByName(FName("RifleSocket")); 
	const USkeletalMeshSocket* HipleftSocket = GetMesh1P()->GetSocketByName(FName("HipLSocket")); 

	if (HipLeftSocket)
	{
		if (WeaponToEquip->WeaponType == EWeaponType::Pistol)
		{
			//FString Banana = TEXT("End Interact");
			//GEngine->AddOnScreenDebugMessage(7, 5.0f, FColor::Red, Banana, 1);

			HipLeftSocket->AttachActor(WeaponToEquip, GetMesh1P());

			WeaponToEquip->GetBoxComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			WeaponToEquip->GetSphereComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			WeaponToEquip->GetItemSkeleton()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		}

		CurrentWeapon = WeaponToEquip;
	}

	if (ClaviculeSocket)
	{
		if (WeaponToEquip->WeaponType == EWeaponType::Rifle)
		{
			ClaviculeSocket->AttachActor(WeaponToEquip, GetMesh1P());

			WeaponToEquip->GetBoxComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			WeaponToEquip->GetSphereComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			WeaponToEquip->GetItemSkeleton()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		}
	}

	if (HipleftSocket)
	{
		if (WeaponToEquip->WeaponType == EWeaponType::SuperLamp)
		{
			HipleftSocket->AttachActor(WeaponToEquip, GetMesh1P());

			WeaponToEquip->GetBoxComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			WeaponToEquip->GetSphereComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			WeaponToEquip->GetItemSkeleton()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		}

	}
	*/

}





