// Fill out your copyright notice in the Description page of Project Settings.


#include "MallProject/TMPAnimInstance.h"


//Custome includes 
#include "MallProjectCharacter.h"

//Engine includes. 
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UTMPAnimInstance::NativeInitializeAnimation()
{
	Character = Cast<AMallProjectCharacter>(TryGetPawnOwner());
	if (!ensure(Character != NULL)) return;
	
}

void UTMPAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (Character == nullptr)
	{
		Character = Cast<AMallProjectCharacter>(TryGetPawnOwner());
	}

	if (Character)
	{
		//Get the lateral speed of the character from the velocity
		FVector velocity = Character->GetVelocity();
		velocity.Z = 0.0f; 

		Speed = velocity.Size();

		//is the character in the air ? 
		//We will not have jump mechanic but either way will be cool to check 
		bIsinAir = Character->GetMovementComponent()->IsFalling();

		//is the character acelerating ?

		if (Character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f)
		{
			bIsAcelerating = true;
		}
		
		else
		{
			bIsAcelerating = false;
		}

		FRotator AimRotation = Character->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(Character->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation,AimRotation).Yaw;

		if (Character->GetVelocity().Size() > 0.0f)
		{
			LastMovementOffsetYaw = MovementOffsetYaw;
		}

		//For the Runing
		if (Character ->bIsJogging)
		{
			bWantsToRun = true;
		}
		else
		{
			bWantsToRun = false;
		}
		
		if (Character ->bHasWeapon)
		{
			bHasAnyweapon = true;

			//For the first gun
			if (Character->bHasWeapon1)
			{
				bHasWeapon1 = true;
			}
			else
			{
				bHasWeapon1 = false;
			}

			//For the second gun

			if (Character->bHasRifle)
			{
				bHasWeapon2 = true;
			}
			else
			{
				bHasWeapon2 = false;
			}

		}

		//bAiming = Character->GetAiming();

		bWalkieTalkie = Character->GetWalkieTalkie();


	}


}
