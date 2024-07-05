// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/TCGeometrySpawner.h"

#include "Actors/TCLeggedSurfaceGeometry.h"

bool ATCGeometrySpawner::TrySpawnGeometry(TSubclassOf<ATCLeggedSurfaceGeometry> InActorToSpawn, const FVector& InLocation)
{
	check(GetWorld());
	if (!InActorToSpawn || InLocation.ContainsNaN())
	{
		return false;
	}

	ATCLeggedSurfaceGeometry* NewActor = GetWorld()->SpawnActor<ATCLeggedSurfaceGeometry>(InActorToSpawn);
	check(NewActor);
	
	if(NewActor->GenerateRandomGeometryValues())
	{
		const FVector Location{ InLocation.X, InLocation.Y,
			InLocation.Z + NewActor->GetLegsGeometryData().Z + NewActor->GetSurfaceGeometryData().Z / 2 };

		TArray<AActor*> OptionalActorsToIgnore;
		if (NewActor->CanRenderGeometryAtLocation(
			{ InLocation.X, InLocation.Y,InLocation.Z + NewActor->GetLegsGeometryData().Z / 2 + 5 + NewActor->GetSurfaceGeometryData().Z /2 },
			 OptionalActorsToIgnore, { 0, 0, 0 }))
		{
			NewActor->SetActorLocation(Location);
			if(NewActor->TryRenderGeometry())
			{
				NewActor->RegisterAllComponents();
				GeometryActors.Emplace(NewActor);
				return true;
			}
		}
	}

	NewActor->Destroy();
	return false;
}
