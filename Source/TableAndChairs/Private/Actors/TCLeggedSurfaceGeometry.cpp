// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/TCLeggedSurfaceGeometry.h"
#include <Algo/AllOf.h>
#include <Algo/AnyOf.h>
#include "Components/TCProceduralCubeComponent.h"
#include "DataAsset/TCGeometryDataAsset.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Components/TCSelectableObjectInfoComponent.h"

ATCLeggedSurfaceGeometry::ATCLeggedSurfaceGeometry()
{
	/**Disable tick since we don't need it, also performance friendly*/
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	Surface = CreateDefaultSubobject<UTCProceduralCubeComponent>("Surface");
	check(Surface);
	SetRootComponent(Surface);

	UpperLeftLeg = CreateDefaultSubobject<UTCProceduralCubeComponent>("Leg1");
	check(UpperLeftLeg);
	UpperLeftLeg->AttachToComponent(Surface, FAttachmentTransformRules::KeepRelativeTransform);
	Legs.Emplace(UpperLeftLeg);

	DownLeftLeg = CreateDefaultSubobject<UTCProceduralCubeComponent>("Leg2");
	check(DownLeftLeg);
	DownLeftLeg->AttachToComponent(Surface, FAttachmentTransformRules::KeepRelativeTransform);
	Legs.Emplace(DownLeftLeg);

	UpperRightLeg = CreateDefaultSubobject<UTCProceduralCubeComponent>("Leg3");
	check(UpperRightLeg);
	UpperRightLeg->AttachToComponent(Surface, FAttachmentTransformRules::KeepRelativeTransform);
	Legs.Emplace(UpperRightLeg);

	DownRightLeg = CreateDefaultSubobject<UTCProceduralCubeComponent>("Leg4");
	check(DownRightLeg);
	DownRightLeg->AttachToComponent(Surface, FAttachmentTransformRules::KeepRelativeTransform);
	Legs.Emplace(DownRightLeg);

	SelectableInfoComponent = CreateDefaultSubobject<UTCSelectableObjectInfoComponent>("Selectable Info Component");
	check(SelectableInfoComponent);

	Id = FGuid::NewGuid();
}

bool ATCLeggedSurfaceGeometry::TryRenderGeometry()
{
	/*If the table surface can't generate its geometry, return false, this operation is not successful*/
	if (!Surface->TryGenerateGeometry(SurfaceGeometryData.X, SurfaceGeometryData.Y,
		SurfaceGeometryData.Z))
	{
		return false;
	}

	/*If any of the legs can't generate its geometry, return false, this operation is not successful*/
	if (Algo::AnyOf(Legs, [this](const TObjectPtr<UTCProceduralCubeComponent>& Leg)
	{
		if (Leg.Get())
		{
			return !Leg->TryGenerateGeometry(LegsGeometryData.X, LegsGeometryData.Y,
				LegsGeometryData.Z);
		}

		return true;
	}))
	{
		return false;
	}

	UpdateComponentsLocations();
	return true;
}

bool ATCLeggedSurfaceGeometry::TryUpdateRenderedGeometry()
{
	/*Try update the surface*/
	const bool bSurfaceUpdated = Surface->TryUpdateGeometry(SurfaceGeometryData.X, SurfaceGeometryData.Y,
		SurfaceGeometryData.Z);

	/*Try update all legs (the reason why is AllOf is because we cannot update just one leg since if the data are valid for one leg,
	 *it must be valid for all of them)*/
	const bool LegsUpdated = Algo::AllOf(Legs, [this](const TWeakObjectPtr<UTCProceduralCubeComponent>& Leg)
	{
		return Leg.IsValid() && Leg->TryUpdateGeometry(LegsGeometryData.X, LegsGeometryData.Y, LegsGeometryData.Z);
	});

	/*If at least one geometry set (surface or all legs) has been updated, this operation is successful
	 *but we need to update its position and its components*/
	if (bSurfaceUpdated || LegsUpdated)
	{
		UpdateComponentsLocations();
		return true;
	}

	return false;
}

bool ATCLeggedSurfaceGeometry::CanRenderGeometryAtLocation(const FVector& InLocationToSpawn,
	 TArray<AActor*>& InOptionalActorsToIgnore, const FVector& InBoxExtent)
{
	const FVector Location = { GetActorLocation().X + InLocationToSpawn.X,
		GetActorLocation().Y + InLocationToSpawn.Y, GetActorLocation().Z / 2 - SurfaceGeometryData.Z / 4 + InLocationToSpawn.Z + 5};
	
	const FVector BoxExtent{ SurfaceGeometryData.X / 2 + InBoxExtent.X, SurfaceGeometryData.Y / 2 + InBoxExtent.Y,
		 LegsGeometryData.Z / 2 + InBoxExtent.Z };
	
	InOptionalActorsToIgnore.Reserve(InOptionalActorsToIgnore.Num() + 1);
	InOptionalActorsToIgnore.Emplace(this);

	TArray<AActor*> ActorsHit;
	UKismetSystemLibrary::BoxOverlapActors(this, Location, BoxExtent, {},
		nullptr, InOptionalActorsToIgnore,ActorsHit);

	if (!ActorsHit.IsEmpty())
	{
		#if WITH_EDITOR
		UKismetSystemLibrary::DrawDebugBox(this, Location, BoxExtent, FLinearColor::Red,
			FRotator::ZeroRotator, .4f, 2.f);
		#endif
		return false;
	}

	return true;
}

void ATCLeggedSurfaceGeometry::ClearGeometryData()
{
	Surface->ClearGeometryData();
	for(auto& Leg : Legs)
	{
		if(Leg)
		{
			Leg->ClearGeometryData();
		}
	}
	
}

void ATCLeggedSurfaceGeometry::SetSelected_Implementation()
{
	bIsFocused = false;
	bIsSelected = true;
	
	const FTCSelectableObjectInfoData& SelectedInfo = SelectableInfoComponent->GetSelectedInfo();
	Internal_ApplyDynamicMaterialInfo(SelectedInfo);
}

void ATCLeggedSurfaceGeometry::SetFocused_Implementation()
{
	bIsFocused = true;
	bIsSelected = false;
	
	const FTCSelectableObjectInfoData& FocusedInfo = SelectableInfoComponent->GetFocusedInfo();
	Internal_ApplyDynamicMaterialInfo(FocusedInfo);
}

void ATCLeggedSurfaceGeometry::SetDefault_Implementation()
{
	bIsFocused = false;
	bIsSelected = false;
	
	const FTCSelectableObjectInfoData& DefaultInfo = SelectableInfoComponent->GetDefaultInfo();
	Internal_ApplyDynamicMaterialInfo(DefaultInfo);
}

void ATCLeggedSurfaceGeometry::UpdateComponentsLocations()
{
	/*Update all components positions*/
	UpperLeftLeg->SetRelativeLocation({ Surface->GetWidth() / 2 - UpperLeftLeg->GetWidth() / 2,
		-Surface->GetLength() / 2 + UpperLeftLeg->GetLength() / 2,
		-Surface->GetHeight() / 2 - UpperLeftLeg->GetHeight() / 2 });
	DownLeftLeg->SetRelativeLocation({ Surface->GetWidth() / 2 - DownLeftLeg->GetWidth() / 2,
		Surface->GetLength() / 2 - DownLeftLeg->GetLength() / 2,
		-Surface->GetHeight() / 2 - DownLeftLeg->GetHeight() / 2 });
	UpperRightLeg->SetRelativeLocation({ -Surface->GetWidth() / 2 + UpperRightLeg->GetWidth() / 2,
		Surface->GetLength() / 2 - UpperRightLeg->GetLength() / 2,
		-Surface->GetHeight() / 2 - UpperRightLeg->GetHeight() / 2 });
	DownRightLeg->SetRelativeLocation({ -Surface->GetWidth() / 2 + DownRightLeg->GetWidth() / 2,
		-Surface->GetLength() / 2 + DownRightLeg->GetLength() / 2,
		-Surface->GetHeight() / 2 - DownRightLeg->GetHeight() / 2 });
}

void ATCLeggedSurfaceGeometry::Internal_ApplyDynamicMaterialInfo(const FTCSelectableObjectInfoData& InMaterialInfo)
{
	auto ApplyMaterialToComponent = [&InMaterialInfo](TWeakObjectPtr<UMaterialInstanceDynamic> InMaterialInstanceDynamic)
	{
		if(InMaterialInstanceDynamic.IsValid())
		{
			InMaterialInstanceDynamic->SetVectorParameterValue(InMaterialInfo.ColorParamName, InMaterialInfo.Color);
			InMaterialInstanceDynamic->SetScalarParameterValue(InMaterialInfo.EmissiveValueParamName, InMaterialInfo.EmissiveColorValue);
		}
	};

	/*Apply parameters to surface component and legs components*/
	ApplyMaterialToComponent(Surface->GetAppliedDynamicMaterialInstance());
	for(const auto& Leg : Legs)
	{
		if(Leg)
		{
			ApplyMaterialToComponent(Leg->GetAppliedDynamicMaterialInstance());
		}
	}
}

bool ATCLeggedSurfaceGeometry::GenerateRandomGeometryValues()
{
	/*If no geometry data asset has been selected, early return*/
	if (!GeometryDataAsset)
	{
		#if WITH_EDITOR
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
				FString::Printf(
					TEXT("Cannot generate Legged Surface Geometry %s due to absent Geometry data asset"),
					*GetName()));
		}
		#endif
		return false;
	}

	/*If the geometry data asset contains invalid data (val <= 0), early return*/
	if (!GeometryDataAsset->IsValid())
	{
		#if WITH_EDITOR
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
				FString::Printf(
					TEXT("Invalid geometry values found on Legged Surface Geometry %s Geometry data asset"),
					*GetName()));
		}
		#endif
		return false;
	}

	auto ComputeRandomValue = [](const FVector2D& RandomRange)
	{
		return FMath::RandRange(RandomRange.X, RandomRange.Y);
	};

	/*Create random values*/
	SurfaceGeometryData = { ComputeRandomValue(GeometryDataAsset->SurfaceXAxisMinMaxValues),
		ComputeRandomValue(GeometryDataAsset->SurfaceYAxisMinMaxValues),
		ComputeRandomValue(GeometryDataAsset->SurfaceZAxisMinMaxValues) };

	LegsGeometryData = { ComputeRandomValue(GeometryDataAsset->LegsXAxisMinMaxValues),
		ComputeRandomValue(GeometryDataAsset->LegsYAxisMinMaxValues),
		ComputeRandomValue(GeometryDataAsset->LegsZAxisMinMaxValues) };
	return true;
}
