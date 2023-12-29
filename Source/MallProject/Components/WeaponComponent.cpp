// Fill out your copyright notice in the Description page of Project Settings.

//System includes:
#include "MallProject/Components/WeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MallProject/MallProjectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"



//Custome Includes:
#include "MallProject/Interactables/WeaponInteractableActor.h"

UWeaponComponent::UWeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
	//Character = Cast<AMallProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	CurrentWeapon = Cast<AWeaponInteractableActor>(GetOwner());
	
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<AMallProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void UWeaponComponent::AttachWeapon(AMallProjectCharacter* TargetCharacter)
{

	//TargetCharacter = Character;
	if (TargetCharacter == nullptr)
	{
		return;
	}

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(TargetCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UWeaponComponent::Fire);
		}
	}
}

void UWeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}


	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}



	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

