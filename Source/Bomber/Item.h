// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Bomber.h"
#include "GameFramework/Actor.h"

#include "Item.generated.h"

UCLASS()
class BOMBER_API AItem final : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

	/** The MapComponent manages this actor on the Level Map */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "C++")
	class UMapComponent* MapComponent;

	UPROPERTY(BlueprintReadOnly, Category = "C++")
	class UStaticMeshComponent* ItemMeshComponent;

	/** Type and its class as associated pairs  */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "C++")
	TMap<EItemTypeEnum, class UStaticMesh*> ItemTypesByMeshes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "C++")
	EItemTypeEnum ItemType = EItemTypeEnum::None;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() final;

	/** Called when an instance of this class is placed (in editor) or spawned */
	virtual void OnConstruction(const FTransform& Transform) final;

	/**
	 * Called when character starts to overlaps the ItemCollisionComponent component
	 * Increases +1 to skate\fire\bomb amount to the character
	 */
	UFUNCTION(BlueprintCallable, Category = "C++")
	void OnItemBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
