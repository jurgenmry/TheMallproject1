// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MallProject/Interactables/InteractableActor.h"
#include "MallProject/AmmoType.h"
#include "Engine/DataTable.h"
#include "MallProject/WeaponType.h"
#include "WeaponInteractableActor.generated.h"


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
	class USkeletalMesh* WeaponMesh;
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


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	int32 SlothIndex;

	//================================================================================//
	// FUNCTIONS
	//================================================================================//

	virtual void OnConstruction(const FTransform& Transform) override;

	AWeaponInteractableActor();

	virtual void Interact(class AMallProjectCharacter* CharacterReference) override;

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
	FORCEINLINE int32 GetSlothIndex() const { return SlothIndex; }


	/******************* Setters *******************/
	/***********************************************/

	FORCEINLINE int32 SeSlothIndex(int32 Index) { return SlothIndex = Index; }


private: 

	//================================================================================//
	// Variables & Properties
	//================================================================================//

	//Ammo count for this weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	int32 AmmoCount;

	//Maximun ammo our weapon can hold
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	int32 MagazineCapacity;


};
