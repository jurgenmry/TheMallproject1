// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MallProject/Interfaces/InteractInterface.h"
#include "InteractableActor.generated.h"



UENUM()
enum class EItemState : uint8
{
	ReadyForPickup UMETA(DisplayName = "Ready_Pickup"),
	EquipInterping UMETA(DisplayName = "EquipInterping"),
	PickedUp UMETA(DisplayName = "PickedUp"),
	Equipped UMETA(DisplayName = "Equipped"),
	NoState UMETA(DisplayName = "NoState") // This is for doors and others
};

UCLASS()
class MALLPROJECT_API AInteractableActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	

	//================================================================================//
	// Variables & Properties
	//================================================================================//

	UPROPERTY(EditAnywhere, Category = "Components")
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	class USkeletalMeshComponent* ItemSkeleton;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UBoxComponent* BoxComps;

	UPROPERTY(EditAnywhere, Category = "Components")
	class USphereComponent* SphereComps;

	UPROPERTY(EditInstanceOnly, Category = "Components")
	FInteractableData InstaceInteractableData;

	UPROPERTY(EditAnywhere,Category = "Components")
	EItemState ItemState;

	

	//================================================================================//
	// FUNCTIONS
	//================================================================================//
	
	AInteractableActor();

	virtual void BeginPlay() override;

	virtual void BeginFocus() override;

	virtual void EndFocus() override;

	virtual void BeginInteract() override;

	virtual void EndInteract() override;

	virtual void Interact(class AMallProjectCharacter* CharacterReference) override;

	void SetItemState(EItemState State);

	/* Sets the itme properties */
	void SetItemProperties(EItemState State);

	//Getters

	FORCEINLINE USkeletalMeshComponent* GetItemSkeleton() const { return ItemSkeleton; }
	FORCEINLINE UBoxComponent* GetBoxComponent() const { return BoxComps; }
	FORCEINLINE USphereComponent* GetSphereComponent() const { return SphereComps;  }
	FORCEINLINE EItemState GetItemState() const { return ItemState; }

	//Setters


protected:
	

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIntex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIntex
		);



};
