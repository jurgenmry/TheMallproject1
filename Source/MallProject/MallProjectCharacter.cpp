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

//Custome Includes
#include "MallProject/UserInterface/MallHud.h"


//////////////////////////////////////////////////////////////////////////
// AMallProjectCharacter

AMallProjectCharacter::AMallProjectCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
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

void AMallProjectCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AMallProjectCharacter::GetHasRifle()
{
	return bHasRifle;
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

bool AMallProjectCharacter::PerformTrace(FHitResult& OutHitResult)
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
		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Visibility);

		if (OutHitResult.bBlockingHit)
		{
			return true;
		}
	}
	return false;
}

void AMallProjectCharacter::TraceForItems()
{
	if (InteractionData.bShouldTraceForItems)
	{
		FHitResult ItemTraceresult;
		PerformTrace(ItemTraceresult);
		if (ItemTraceresult.bBlockingHit)
		{
			//InteractionData.CurrentIntertactable = Cast<AActor>(ItemTraceresult.GetActor());
			//FoundInteractable(InteractionData.CurrentIntertactable);

			//if the class implements the interact interface check item
			if (ItemTraceresult.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
			{
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
		}
	}

	NoInteractableFound();
}

void AMallProjectCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (InteractionData.bIsInteracting == true)
	{
		//UE_LOG(LogTemp, Warning, TEXT("InteractionData.bIsInteracting == true"));

		EndInteract();
	}

	if (InteractionData.CurrentIntertactable)
	{
		TargetInteractable = InteractionData.CurrentIntertactable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentIntertactable = NewInteractable;
	TargetInteractable = NewInteractable;

	InteractionData.bIsInteracting = true; // check this line of code. Might be needed to remove it 

	//HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus(); //What happend after beggins focus? Where are we getting redirected ?
}

void AMallProjectCharacter::NoInteractableFound()
{
	if (InteractionData.bIsInteracting == true)
	{
		InteractionData.bIsInteracting = false;
	}

	if (InteractionData.CurrentIntertactable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		//HUD->HideInteractionWidget();

		InteractionData.CurrentIntertactable = nullptr;
		TargetInteractable = nullptr;
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
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
					this,
					&AMallProjectCharacter::Interact,
					TargetInteractable->InteractableData.InteractionDuration,
					false);
			}
		}
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
	}
}








