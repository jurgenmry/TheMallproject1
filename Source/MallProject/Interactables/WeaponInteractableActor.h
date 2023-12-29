// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MallProject/Interactables/InteractableActor.h"
#include "MallProject/AmmoType.h"
#include "Engine/DataTable.h"
#include "WeaponInteractableActor.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Pistol UMETA(DisplayName = "Pistol"),
	Rifle UMETA(DisplayName = "Rifle"), 
	Shotgun UMETA(DisplayName = "Shotgun"),
	Melee UMETA(DisplayName = "Melee"),			
	SuperLamp UMETA(DisplayName = "SuperLamp"), 
	Other UMETA(DisplayName = "Other")
};

USTRUCT(BlueprintType)
struct FWeaponDataTable : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* PickUpSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* ItemMesh;

};

UCLASS()
class MALLPROJECT_API AWeaponInteractableActor : public AInteractableActor
{
	GENERATED_BODY()

public:

	//================================================================================//
	// Variables & Properties
	//================================================================================//

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	//class UWeaponComponent* WeaponComps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	EAmmoType AmmoType;

	//Fname for the reload Montage section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FName ReloadMontageSection;

	//Datatable for weapon Properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data Table")
	UDataTable* WeaponDatatable;

	//================================================================================//
	// FUNCTIONS
	//================================================================================//

	AWeaponInteractableActor();

	virtual void Interact(class AMallProjectCharacter* CharacterReference) override;

	virtual void BeginInteract() override;

	virtual void EndInteract() override;

	/*Called from the character when firing a weapon*/
	void DecrementAmmo();

	void ReloadAmmo(int32 Ammount);

	/******************* Getters *******************/
	/***********************************************/

	FORCEINLINE int32 GetAmmoCount() const { return AmmoCount; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType;  }
	FORCEINLINE FName GetReloadMontageSection() const { return ReloadMontageSection; }
	FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity;  }

	/******************* Setters *******************/
	/***********************************************/



private: 

	//================================================================================//
	// Variables & Properties
	//================================================================================//

	//Ammo count for this weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 AmmoCount;

	//Maximun ammo our weapon can hold
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 MagazineCapacity;
};
