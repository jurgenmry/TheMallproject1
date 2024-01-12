// Fill out your copyright notice in the Description page of Project Settings.


#include "MallProject/Interactables/InteractableActor.h"

//System includes:

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

//Custome includes:
#include "MallProject/UserInterface/MallHud.h"
#include "MallProject/MallProjectCharacter.h"
#include "MallProject/UserInterface/InteractWidget.h"

AInteractableActor::AInteractableActor()
	//:Type(EInteractableType::Pickup)
	:ItemState(EItemState::NoState)
{

	PrimaryActorTick.bCanEverTick = false;


	BoxComps = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComps"));
	//BoxComps->SetupAttachment(GetRootComponent());
	BoxComps->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComps->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	SetRootComponent(BoxComps);
	BoxComps->SetHiddenInGame(true);

	ItemSkeleton = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemSkeleton"));
	ItemSkeleton->SetupAttachment(GetRootComponent());
	ItemSkeleton->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemSkeleton->SetupAttachment(GetRootComponent());
	
	//BoxComps->SetHiddenInGame(true);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(GetRootComponent());
	

	SphereComps = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComps"));
	SphereComps->SetSphereRadius(70.0f);
	SphereComps->SetupAttachment(GetRootComponent());
	SphereComps->SetHiddenInGame(true);

	InstaceInteractableData.InteractableType = EInteractableType::Pickup;
}


void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstaceInteractableData;

	SphereComps->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnSphereOverlap);
	SphereComps->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::OnSphereEndOverlap);

	//Set Item Properties based on itemState;
	SetItemProperties(ItemState);
}



void AInteractableActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIntex, bool bFromSweep, const FHitResult& SweepResult)
{
	FString Overlapping = "Overlaping Interactable";
	GEngine->AddOnScreenDebugMessage(10, 3.0f, FColor::Red, Overlapping, 1);

	if (OtherActor != this && OtherActor)
	{
		AMallProjectCharacter* Character = Cast<AMallProjectCharacter>(OtherActor);
		if (Character)
		{
			if (InstaceInteractableData.InteractableType == EInteractableType::Pickup || InstaceInteractableData.InteractableType == EInteractableType::Weapon)
			{
				ItemState = EItemState::Ready_For_Pickup;
				SetItemState(ItemState);
			}

			Character->InteractionData.bShouldTraceForItems = true;
			Character->HUD->GetInteractionWidget()->SetVisibility(ESlateVisibility::Visible);
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
			//Show a widget
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
			//Stop Showing widget
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
		if (GetPickUpSound())
		{
			UGameplayStatics::PlaySound2D(this, GetPickUpSound());
		}
	}
}

void AInteractableActor::SetItemState(EItemState State)
{
	ItemState = State;
	SetItemProperties(State);
}

void AInteractableActor::SetItemProperties(EItemState State)
{
	switch (State)
	{
		case EItemState::Ready_For_Pickup:

			/*Set Collision Properties for the mesh
			if (ItemMesh)
			{
				ItemMesh->SetSimulatePhysics(false);
				ItemMesh->SetVisibility(true);
				ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
				//ItemMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			}
			*/

			//Set Collision Properties for the  SkeletalMesh
			ItemSkeleton->SetSimulatePhysics(false);
			ItemSkeleton->SetVisibility(true);
			ItemSkeleton->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			ItemSkeleton->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			//Set Collision properties for the area sphere 
			SphereComps->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
			SphereComps->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

			//Set Colision properties for the collision box
			BoxComps->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			BoxComps->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
			BoxComps->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

			break;

		case EItemState::Equip_Interping:
			break;

		case EItemState::Item_PickedUp:
			//Set Collision Properties for the  SkeletalMesh
			ItemSkeleton->SetSimulatePhysics(false);
			ItemSkeleton->SetVisibility(false);
			ItemSkeleton->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			ItemSkeleton->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			//Set Collision properties for the area sphere 
			SphereComps->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			SphereComps->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			//Set Colision properties for the collision box
			BoxComps->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			BoxComps->SetCollisionEnabled(ECollisionEnabled::NoCollision);



			break;

		case EItemState::Equipped:

			//Set Collision Properties for the  SkeletalMesh
			
			ItemSkeleton->SetSimulatePhysics(false);
			ItemSkeleton->SetVisibility(false);
			ItemSkeleton->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			ItemSkeleton->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			//Set Collision properties for the area sphere 
			SphereComps->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			SphereComps->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			//Set Colision properties for the collision box
			BoxComps->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			BoxComps->SetCollisionEnabled(ECollisionEnabled::NoCollision);


			//Set Collision Properties for the mesh
			//ItemMesh->SetSimulatePhysics(false);
			//ItemMesh->SetVisibility(true);
			//ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			//ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			

			//GetBoxComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			//GetSphereComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			//GetItemSkeleton()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			
			break;

		case EItemState::NoState:

			break;

		default:
			break;
	}
	
}


