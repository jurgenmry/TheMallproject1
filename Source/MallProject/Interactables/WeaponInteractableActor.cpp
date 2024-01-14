// Fill out your copyright notice in the Description page of Project Settings.


#include "MallProject/Interactables/WeaponInteractableActor.h"

//System includes
#include "MallProject/Components/WeaponComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

//Custome includes

#include "MallProject/MallProjectCharacter.h"


void AWeaponInteractableActor::OnConstruction(const FTransform& Transform)
{
	const FString WeaponDTPath{ TEXT("/Script/Engine.DataTable'/Game/_Game/DataTables/WeaponDT.WeaponDT'") };
	UDataTable* WeaponTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *WeaponDTPath));
	if (WeaponTableObject)
	{
		FWeaponDataTable* WeaponDataRow = nullptr;

		switch (WeaponType)
		{
		case EWeaponType::Pistol:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("Pistol"), TEXT(""));
			break;
		case EWeaponType::Rifle:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("AssaultRifle"), TEXT(""));
			break;
		case EWeaponType::Shotgun:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("Shotgun"), TEXT(""));
			break;
		case EWeaponType::Melee:
			break;
		case EWeaponType::SuperLamp:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("SuperLight"), TEXT(""));
			break;
		case EWeaponType::Other:
			break;
		default:
			break;
		}

		if (WeaponDataRow)
		{
			AmmoType = WeaponDataRow->AmmoType;
			AmmoCount = WeaponDataRow->WeaponAmmo;
			MagazineCapacity = WeaponDataRow->MagazineCapacity;
			SetPickUpSound(WeaponDataRow->PickUpSound);
			SetEquipUpSound(WeaponDataRow->EquipSound);
			GetItemSkeleton()->SetSkeletalMesh(WeaponDataRow->WeaponMesh);
		}

	}

}

AWeaponInteractableActor::AWeaponInteractableActor()

	: ReloadMontageSection(FName(TEXT("Reload_Pistol")))
	, SlothIndex (0)
{
	//WeaponComps = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComps"));
	WeaponType = EWeaponType::Pistol;
	
	//ItemState = EItemState::ReadyForPickup;
}

void AWeaponInteractableActor::Interact(AMallProjectCharacter* CharacterReference)
{
	Super::Interact(CharacterReference);

	/*
	//GetBoxComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//GetSphereComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	
	//FString InteractWeapon = "InteractWithWeapon";
	//GEngine->AddOnScreenDebugMessage(10, 5.0f, FColor::Red, InteractWeapon, 1);
	//WeaponComps->AttachWeapon(CharacterReference);
	*/

	//testing for the interaction stage of weapon.
	CharacterReference->SetEquippedWeapon(this);
	if (this->GetPickUpSound())
	{
		UGameplayStatics::PlaySound2D(CharacterReference, GetPickUpSound());
	}

	this->SeSlothIndex(CharacterReference->GetInventory().Num());
	this->SetItemState(EItemState::Item_PickedUp);

}


void AWeaponInteractableActor::DecrementAmmo()
{
	if (AmmoCount - 1 <= 0 )
	{
		AmmoCount = 0;
	}
	else
	{
		--AmmoCount;
	}
}

void AWeaponInteractableActor::ReloadAmmo(int32 Ammount)
{

	AmmoCount += Ammount;
}
