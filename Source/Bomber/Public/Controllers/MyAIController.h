﻿// Copyright 2020 Yevhenii Selivanov.

#pragma once

#include "AIController.h"
#include "Bomber.h"
#include "Cell.h"
//---
#include "MyAIController.generated.h"

/**
 * Characters controlled by bots
 */
UCLASS()
class BOMBER_API AMyAIController final : public AAIController
{
	GENERATED_BODY()

public:
	/** Sets default values for this character's properties */
	AMyAIController();

	/** Makes AI go toward specified destination cell */
	UFUNCTION(BlueprintCallable, Category = "C++")
	void MoveToCell(const struct FCell& DestinationCell);

protected:
	/**  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++", meta = (BlueprintProtected, DisplayName = "AI Update Handle"))
	FTimerHandle AIUpdateHandleInternal;

	/** Cell position of current path segment's end */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "C++", meta = (BlueprintProtected, ShowOnlyInnerProperties))
	struct FCell AiMoveTo;	//[G]

	/** Controlled character */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "C++", meta = (BlueprintProtected))
	class APlayerCharacter* MyCharacter;  //[G]

	/** Called when an instance of this class is placed (in editor) or spawned */
	virtual void OnConstruction(const FTransform& Transform) override;

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Allows the PlayerController to set up custom input bindings. */
	virtual void OnPossess(APawn* InPawn) override;

	/** The main AI logic */
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (BlueprintProtected))
    void UpdateAI();

	/** */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++", meta = (BlueprintProtected))
    void OnGameStateChanged(ECurrentGameState CurrentGameState);
};
