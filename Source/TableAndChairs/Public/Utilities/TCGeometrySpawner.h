// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "Actors/TCLeggedSurfaceGeometry.h"
#include <Kismet/BlueprintFunctionLibrary.h>
#include "TCGeometrySpawner.generated.h"

UCLASS()
class TABLEANDCHAIRS_API ATCGeometrySpawner : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * @brief Spawns a geometry actor (the actor must be a subclass of legged surface) and trigger its logic.
	 * Check if the actor can be spawned at the given location, then generates and renders its geometry.
	 * @param InActorToSpawn Actor class to spawn
	 * @param InLocation Location where to spawn the actor
	 * @return True if the operation is successful, false otherwise
	 */
	UFUNCTION(BlueprintCallable)
	bool TrySpawnGeometry(TSubclassOf<ATCLeggedSurfaceGeometry> InActorToSpawn,
		const FVector& InLocation);
	
	/*Default Cube Geometry Data*/
	inline static TArray<int32> CubeGeometryDefaultTriangles
	{
		/*Front*/
		0, 2, 1,
		2, 0, 3,
		/*Back*/
		6, 4, 5,
		6, 7, 4,
		/*Right*/
		8, 10, 9,
		10, 8, 11,
		/*Up*/
		12, 14, 13,
		14, 12, 15,
		/*Left*/
		16, 18, 17,
		18, 16, 19,
		/*Down*/
		20, 22, 21,
		22, 20, 23
	};

	/*Default Cube Geometry Data*/
	inline static TArray<FVector> CubeGeometryDefaultNormals
	{
		/*Front*/
		{ 1,-1, 1},
		{ 1, 1, 1},
		{-1, 1, 1},
		{-1,-1, 1},
		/*Back*/
		{ 1,-1,-1},
		{ 1, 1,-1},
		{-1, 1,-1},
		{-1,-1,-1},
		/*Right*/
		{ 1, 1,-1},
		{ 1, 1, 1},
		{ 1,-1, 1},
		{ 1,-1,-1},
		/*Up*/
		{-1, 1,-1},
		{-1, 1, 1},
		{ 1, 1, 1},
		{ 1, 1,-1},
		/*Left*/
		{-1,-1,-1},
		{-1,-1, 1},
		{-1, 1, 1},
		{-1, 1,-1},
		/*Down*/
		{ 1,-1,-1},
		{ 1,-1, 1},
		{-1,-1, 1},
		{-1,-1,-1},
	};

	/*Default Cube Geometry Data*/
	inline static TArray<FVector2D> CubeGeometryDefaultUV0
	{
			{1, 0}, {1, 1}, {0, 1}, {0, 0},
			{1, 0}, {1, 1}, {0, 1}, {0, 0},
			{1, 0}, {1, 1}, {0, 1}, {0, 0},
			{1, 0}, {1, 1}, {0, 1}, {0, 0},
			{1, 0}, {1, 1}, {0, 1}, {0, 0},
			{1, 0}, {1, 1}, {0, 1}, {0, 0}
	};

private:
	UPROPERTY()
	TArray<TObjectPtr<ATCLeggedSurfaceGeometry>> GeometryActors;
};


