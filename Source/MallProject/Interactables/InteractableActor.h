// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MallProject/Interfaces/InteractInterface.h"
#include "InteractableActor.generated.h"

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
	class UBoxComponent* BoxComps;

	UPROPERTY(EditAnywhere, Category = "Components")
	class USphereComponent* SphereComps;

	UPROPERTY(EditInstanceOnly, Category = "Components")
	FInteractableData InstaceInteractableData;


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
