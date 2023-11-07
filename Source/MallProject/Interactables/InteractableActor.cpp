// Fill out your copyright notice in the Description page of Project Settings.


#include "MallProject/Interactables/InteractableActor.h"

//System includes:

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"

//Custome includes:
#include "MallProject/UserInterface/MallHud.h"
#include "MallProject/MallProjectCharacter.h"
#include "MallProject/UserInterface/InteractWidget.h"

AInteractableActor::AInteractableActor()
	//:Type(EInteractableType::Pickup)
{

	PrimaryActorTick.bCanEverTick = false;

	BoxComps = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComps"));
	//BoxComps->SetupAttachment(GetRootComponent());
	SetRootComponent(BoxComps);
	BoxComps->SetHiddenInGame(true);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(BoxComps);
	ItemMesh->SetSimulatePhysics(true);

	SphereComps = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComps"));
	SphereComps->SetSphereRadius(70.0f);
	SphereComps->SetupAttachment(GetRootComponent());

	ItemSkeleton = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemSkeleton"));
	ItemSkeleton->SetupAttachment(GetRootComponent());
	//ItemSkeleton->SetSimulatePhysics(true);
	InstaceInteractableData.InteractableType = EInteractableType::Pickup;
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
			Character->HUD->GetInteractionWidget()->SetVisibility(ESlateVisibility::Visible);
		
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
			Character->HUD->GetInteractionWidget()->SetVisibility(ESlateVisibility::Collapsed);
			//UE_LOG(LogTemp, Warning, TEXT("Removing Shader mat"));
		}
	}
}

void AInteractableActor::BeginFocus()
{
	
	if (ItemMesh)
	{
		switch (InstaceInteractableData.InteractableType)
		{
		case EInteractableType::Pickup:
			ItemMesh->SetRenderCustomDepth(true);
			//ItemSkeleton->SetRenderCustomDepth(true);
			break;
		case EInteractableType::Weapon:
			//ItemSkeleton->SetRenderCustomDepth(true);
			break;
		case EInteractableType::NonPlayableCharacter:
			break;
		case EInteractableType::Device:
			break;
		case EInteractableType::Toggle:
			break;
		case EInteractableType::Container:
			break;
		default:
			break;
		}
		
	}
	
}

void AInteractableActor::EndFocus()
{
	
	if (ItemMesh)
	{
		switch (InstaceInteractableData.InteractableType)
		{
		case EInteractableType::Pickup :
			ItemMesh->SetRenderCustomDepth(false);
			//ItemSkeleton->SetRenderCustomDepth(false);
			break;
		case EInteractableType::Weapon:
			//ItemSkeleton->SetRenderCustomDepth(false);
			break;
		case EInteractableType::NonPlayableCharacter:
			break;
		case EInteractableType::Device:
			break;
		case EInteractableType::Toggle:
			break;
		case EInteractableType::Container:
			break;
		default:
			break;
		}

	}
	
}

void AInteractableActor::BeginInteract()
{
	//UE_LOG(LogTemp, Warning, TEXT("Calling begin interact Override on interface test actor"));

	
}

void AInteractableActor::EndInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling End interact Override on interface test actor"));
}


void AInteractableActor::Interact(AMallProjectCharacter* CharacterReference)
{

	if (CharacterReference)
	{
		//FString textString = "Interaction Succesful"; 
		//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, textString, 1);
		//Destroy();
	}
}


