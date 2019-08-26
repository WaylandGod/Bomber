// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Bomber.h"
#include "Cell.h"
#include "GameFramework/Actor.h"

#include "GeneratedMap.generated.h"

/**
 * Procedurally generated grid of cells and actors on the scene
 * @see USingletonLibrary::LevelMap_ reference to this Level Map
 */
UCLASS()
class BOMBER_API AGeneratedMap final : public AActor
{
	GENERATED_BODY()

public:
	/** The blueprint background actor  */
	UPROPERTY(BlueprintReadWrite, Category = "C++")
	UChildActorComponent* BackgroundBlueprintComponent;

	/** The blueprint class with the background, collision cage and floor. Can be changed in the editor */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "C++")
	TSubclassOf<AActor> BackgroundBlueprintClass;

	/** The unique set of player characters */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "C++")
	TArray<class AMyCharacter*> CharactersOnMap;

	/** Number of characters on the Level Map */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "C++")
	int32 CharactersNumber = 4;

	/* ---------------------------------------------------
	 *					Level map public functions
	 * --------------------------------------------------- *

	/** Sets default values for this actor's properties */
	AGeneratedMap();

	/**
	 * Getting an array of cells by four sides of an input center cell and type of breaks.
	 * 
	 * @param Cell The start of searching by the sides
	 * @param SideLength Length of each side
	 * @param Pathfinder Type of cells searching
	 * @return Found cells
	 * @todo to C++ GetSidesCells(...)
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure, Category = "C++", meta = (AutoCreateRefTerm = "SideLength"))
	TSet<struct FCell> GetSidesCells(
		const struct FCell& Cell,
		EPathTypesEnum Pathfinder,
		const int32& SideLength = 100) const;

	/**
	 * The intersection of input cells and actors of the specific type on these cells.
	 * (Cells ∩ Actors type)  
	 *
	 * @param Cells The cells set to intersect
	 * @param ActorsTypesBitmask EActorTypeEnum bitmask to intersect
	 * @param ExcludePlayer 
	 * @return The set that contains all cells by actor types
	 * @todo to C++ IntersectionCellsByTypes(...)
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure, Category = "C++", meta = (AdvancedDisplay = 2, AutoCreateRefTerm = "ActorsTypesBitmask"))
	TSet<struct FCell> IntersectionCellsByTypes(
		const TSet<struct FCell>& Cells,
		UPARAM(meta = (Bitmask, BitmaskEnum = EActorTypeEnum)) const int32& ActorsTypesBitmask,
		const class AMyCharacter* ExcludePlayer = nullptr) const;

	/**
	 * Spawns a level actor on the Level Map by the specified type. Then calls AddActorToGridArray().
	 * 
	 * @param Type Which type of level actors
	 * @param Cell Actors location
	 * @return Spawned actor on the Level Map, nullptr otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (AutoCreateRefTerm = "Type,Cell"))
	AActor* SpawnActorByType(const EActorTypeEnum& Type, const FCell& Cell);

	/**
	 * The function that places the actor on the Level Map, attaches it and writes this actor to the GridArray_

	 * @param Cell The location where the child actor will be standing on
	 * @param UpdateActor The spawned or dragged PIE actor
	 */
	UFUNCTION(BlueprintCallable, Category = "C++")
	void AddActorToGridArray(const struct FCell& Cell, AActor* UpdateActor);

	/** Find and remove only this input actor-value of the cell-key from the Grid Array */
	UFUNCTION(BlueprintCallable, Category = "C++")
	void RemoveActorFromGridArray(const AActor* Actor);

	/**
	 * Destroy all actors from the set of cells
	 * @param Keys The set of cells for destroying the found actors
	 * @todo to C++ DestroyActorsFromMap(...)
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++")
	void DestroyActorsFromMap(const TSet<struct FCell>& Keys);

	/**
	 * Check if the Level Map contains the specified cell.
	 *
	 * @param Cell The Cell to check for.
	 * @param ExcludeActor The Cell can contains this actor.
	 * @return true if the Level Map contains the cell.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++", meta = (AdvancedDisplay = 1, AutoCreateRefTerm = "Cell"))
	FORCEINLINE bool IsEmptyCell(const struct FCell& Cell, const AActor* ExcludeActor = nullptr) const
	{
		const auto FoundActor = GridArray_.Find(Cell);
		return FoundActor						   // The key contains in the level map
			   && (*FoundActor == ExcludeActor	 // Is the same exclude actor
					  || !IS_VALID(*FoundActor));  // The level map value is empty
	}

protected:
	/** @ingroup actors_management
	 * Storage of cells and their actors
	 * @see GenerateLevelMapGe()
	 */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "C++", meta = (DisplayName = "Grid Array"))
	TMap<struct FCell, const AActor*> GridArray_;

	/** The chance of boxes generation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "C++")
	int32 BoxesChance_ = 50;

	/* ---------------------------------------------------
	 *					Level map protected functions
	 * --------------------------------------------------- */

	/** Called when an instance of this class is placed (in editor) or spawned
	 * @todo Generate only platform without boxes*/
	virtual void OnConstruction(const FTransform& Transform) final;

	/** This is called only in the gameplay before calling begin play to generate level actors */
	virtual void PostInitializeComponents() final;

	/** Spawns and fills the Grid Array values by level actors */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++", meta = (AutoCreateRefTerm = "ActorsTypesBitmask,ActorLocationToSpawn"))
	void GenerateLevelActors();

	/* ---------------------------------------------------
	 *					Editor development
	 * --------------------------------------------------- */

#if WITH_EDITORONLY_DATA
	/** Mark the editor updating visualization(text renders) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "C++")
	bool bShouldShowRenders;
#endif  //WITH_EDITORONLY_DATA [Editor]

	/** Destroys all attached level actors
	 *
	 * @param bIsEditorOnly if should destroy editor-only actors that were spawned in the PIE world
	 */
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (DevelopmentOnly))
	void DestroyAttachedActors(bool bIsEditorOnly = false);

#if WITH_EDITOR  // Destroyed() [Editor]
	/** Called when this actor is explicitly being destroyed during gameplay or in the editor, not called during level streaming or gameplay ending */
	virtual void Destroyed() final;
#endif  // Destroyed() [Editor]
};
