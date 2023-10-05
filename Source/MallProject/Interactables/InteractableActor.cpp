// Fill out your copyright notice in the Description page of Project Settings.


#include "MallProject/Interactables/InteractableActor.h"

//System includes:

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

//Custome includes:

#include "MallProject/MallProjectCharacter.h"

AInteractableActor::AInteractableActor()
{

	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);
	ItemMesh->SetSimulatePhysics(true);

	BoxComps = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComps"));
	BoxComps->SetupAttachment(GetRootComponent());

	SphereComps = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComps"));
	SphereComps->SetSphereRadius(70.0f);
	SphereComps->SetupAttachment(GetRootComponent());

}


void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstaceInteractableData;

	SphereComps->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnSphereOverlap);
	SphereComps->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::OnSphereEndOverlap);
	
}



void AInteractableActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIntex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherActor)
	{
		AMallProjectCharacter* Character = Cast<AMallProjectCharacter>(OtherActor);
		if (Character)
		{
			Character->InteractionData.bShouldTraceForItems = true;
			//UE_LOG(LogTemp, Warning, TEXT("Applying Shader mat"));
		}
	}
}

void AInteractableActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIntex)
{
	if (OtherActor != this && OtherActor)
	{
		AMallProjectCharacter* Character = Cast<AMallProjectCharacter>(OtherActor);
		if (Character)
		{
			Character->InteractionData.bShouldTraceForItems = false;
			//UE_LOG(LogTemp, Warning, TEXT("Removing Shader mat"));
		}
	}
}

void AInteractableActor::BeginFocus()
{
	if (ItemMesh)
	{
		ItemMesh->SetRenderCustomDepth(true);
	}
}

void AInteractableActor::EndFocus()
{
	if (ItemMesh)
	{
		ItemMesh->SetRenderCustomDepth(false);
	}
}

void AInteractableActor::BeginInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling begin interact Override on interface test actor"));

	//
}

void AInteractableActor::EndInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling End interact Override on interface test actor"));
}


void AInteractableActor::Interact(AMallProjectCharacter* CharacterReference)
{

	if (CharacterReference)
	{
		//TakePickUp(CharacterReference);
	}
}


