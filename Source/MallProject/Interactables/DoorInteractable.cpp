// Fill out your copyright notice in the Description page of Project Settings.


#include "MallProject/Interactables/DoorInteractable.h"

//System includes
#include "Kismet/KismetMathLibrary.h"
#include "Curves/CurveFloat.h"


ADoorInteractable::ADoorInteractable()
	: bOpen(false)
	, ReadyState(true)
{
	PrimaryActorTick.bCanEverTick = true;
};


void ADoorInteractable::BeginPlay()
{
	Super::BeginPlay();
	RotateValue = 1.0f;

	if (DoorCurve)
	{
		FOnTimelineFloat TimeCallback;
		FOnTimelineEventStatic TimeLineFinishedCallback;

		TimeCallback.BindUFunction(this, FName("RotateDoor"));
		TimeLineFinishedCallback.BindUFunction(this, FName("SetDoorState"));

		MyTimeLine.AddInterpFloat(DoorCurve, TimeCallback);
		MyTimeLine.SetTimelineFinishedFunc(TimeLineFinishedCallback);


	}
};

void ADoorInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//DoorRotation = GetItemMesh()->GetRelativeRotation();
	//RotateDoor(bOpen);

	MyTimeLine.TickTimeline(DeltaTime);
};

void ADoorInteractable::RotateDoor()
{
	/*
	if (bOpen)
	{
		GetItemMesh()->SetRelativeRotation(
			FMath::Lerp(FQuat(DoorRotation), FQuat(FRotator(0.0f, RotateValue, 0.0f)), 0.01));
	}
	else
	{
		GetItemMesh()->SetRelativeRotation(
			FMath::Lerp(FQuat(DoorRotation), FQuat(FRotator(0.0f, 0.0f, 0.0f)), 0.01));
	}
	*/

	TimelineValue = MyTimeLine.GetPlaybackPosition();
	CurveFloatValue = RotateValue * DoorCurve->GetFloatValue(TimelineValue);

	FQuat NewRotation = FQuat(FRotator(0.0f, CurveFloatValue, 0.0f));

	GetItemMesh()->SetRelativeRotation(NewRotation);

}
void ADoorInteractable::SetDoorState()
{
	ReadyState = true;
};

void ADoorInteractable::Interact(class AMallProjectCharacter* CharacterReference)
{
	if (CharacterReference)
	{
		if (ReadyState)
		{
			bOpen = !bOpen;

			FVector PawnLocation = CharacterReference->GetActorLocation();
			FVector Direction = GetActorLocation() - PawnLocation;
			Direction = UKismetMathLibrary::LessLess_VectorRotator(Direction, GetActorRotation());

			DoorRotation = GetItemMesh()->GetRelativeRotation();
			if (bOpen)
			{
				if (Direction.X > 0.0f)
				{
					RotateValue = 1.0f;
				}
				else
				{
					RotateValue = -1.0f;
				}

				ReadyState = false;
				MyTimeLine.PlayFromStart();
			}

			else
			{
				ReadyState = false;
				MyTimeLine.Reverse();

			}
		}
	}
};


void ADoorInteractable::BeginFocus()
{
	
};

void ADoorInteractable::EndFocus()
{
};
void ADoorInteractable::BeginInteract()
{
};
void ADoorInteractable::EndInteract()
{
};
