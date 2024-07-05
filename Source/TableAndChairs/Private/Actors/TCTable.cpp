// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TCTable.h"

#include "Actors/TCChair.h"
#include "Components/TCChairPoolComponent.h"
#include "Components/TCProceduralCubeComponent.h"
#include "DataAsset/TCChairGeometryDataAsset.h"
#include "DataAsset/TCGeometryDataAsset.h"
#include <Kismet/GameplayStatics.h>

#include "Actors/TCHUD.h"

ATCTable::ATCTable()
{
	ChairPoolComponent = CreateDefaultSubobject<UTCChairPoolComponent>("ChairPoolComponent");
	check(ChairPoolComponent);
}

bool ATCTable::TryRenderGeometry()
{
	if (!Super::TryRenderGeometry())
	{
		return false;
	}

	Internal_ComputeAndRegenerateChairs();
	return true;
}

bool ATCTable::TryUpdateRenderedGeometry()
{
	if (!Super::TryUpdateRenderedGeometry())
	{
		return false;
	}

	SetActorLocation(GetActorLocation() + FVector(0, 0,
		LegsGeometryData.Z + SurfaceGeometryData.Z / 2 - GetActorLocation().Z));
	
	Internal_ComputeAndRegenerateChairs();
	return true;
}

bool ATCTable::CanRenderGeometryAtLocation(const FVector& InLocationToSpawn,
	 TArray<AActor*>& InOptionalActorsToIgnore, const FVector& InBoxExtent)
{
	const FVector ChairsPadding = { ChairsSeatGeometry.X + InBoxExtent.X + ChairsPaddingFromTable,
		ChairsSeatGeometry.X + InBoxExtent.Y + ChairsPaddingFromTable, InBoxExtent.Z };
	
	InOptionalActorsToIgnore.Reserve(InOptionalActorsToIgnore.Num() + Chairs.Num() + 1);

	/*Add the player pawn as an actor to ignore*/
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	check(PlayerController);
	InOptionalActorsToIgnore.Emplace(PlayerController->GetPawn());

	Algo::TransformIf(Chairs, InOptionalActorsToIgnore, [](const TWeakObjectPtr<ATCChair>& Chair)
		{
			return Chair.IsValid();
		},
		[](const TWeakObjectPtr<ATCChair>& Chair)
		{
			return Chair.Get();
		});

	if(!Super::CanRenderGeometryAtLocation(InLocationToSpawn,InOptionalActorsToIgnore,ChairsPadding))
	{
		ATCHUD* HUD = Cast<ATCHUD>(PlayerController->GetHUD());
		HUD->OnRenderFailed();
		return false;
	}

	return true;
}

bool ATCTable::GenerateRandomGeometryValues()
{
	/*If no geometry data asset has been selected in the chair pool component, early return*/
	if (!ChairPoolComponent->GetChairGeometryDataAsset())
	{
		#if WITH_EDITOR
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
				FString::Printf(
					TEXT(
						"Cannot generate Legged Surface Geometry %s due to absent Chair Geometry data asset in %s"),
					*GetName(), *ChairPoolComponent->GetName()));
		}
		#endif
		return false;
	}

	const UTCChairGeometryDataAsset* ChairGeometryDataAsset = ChairPoolComponent->GetChairGeometryDataAsset();
	/*If the geometry data asset in the chair pool component contains invalid data (val <= 0), early return*/
	if (!ChairGeometryDataAsset->IsValid())
	{
		#if WITH_EDITOR
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor::Red,
				FString::Printf(
					TEXT("Invalid geometry values found on %s Chair Geometry data asset"),
					*ChairPoolComponent->GetName()));
		}
		#endif
		return false;
	}

	if (!Super::GenerateRandomGeometryValues())
	{
		return false;
	}

	auto ComputeRandomValue = [](const FVector2D& RandomRange)
	{
		return FMath::RandRange(RandomRange.X, RandomRange.Y);
	};

	ChairsSeatGeometry = {
		ComputeRandomValue(ChairGeometryDataAsset->SurfaceXAxisMinMaxValues),
		ComputeRandomValue(ChairGeometryDataAsset->SurfaceYAxisMinMaxValues),
		ComputeRandomValue(ChairGeometryDataAsset->SurfaceZAxisMinMaxValues) };

	ChairsLegsGeometry = { ComputeRandomValue(ChairGeometryDataAsset->LegsXAxisMinMaxValues),
		ComputeRandomValue(ChairGeometryDataAsset->LegsYAxisMinMaxValues),
		ComputeRandomValue(ChairGeometryDataAsset->LegsZAxisMinMaxValues) };

	ChairsBackGeometry = { ComputeRandomValue(ChairGeometryDataAsset->BackXAxisMinMaxValues),
		ComputeRandomValue(ChairGeometryDataAsset->BackYAxisMinMaxValues),
		ComputeRandomValue(ChairGeometryDataAsset->BackZAxisMinMaxValues) };

	ChairsSpacing = FMath::RandRange(ChairGeometryDataAsset->Spacing.X,
		ChairGeometryDataAsset->Spacing.Y);
	return true;
}

bool ATCTable::TryUpdateChairsGeometry(const FVector& InSeatNewGeometry, const double& InLegsNewHeight,
	const double& InBackNewHeight, const double& InNewSpacing)
{
	if (ChairsSeatGeometry != InSeatNewGeometry || ChairsLegsGeometry.Z != InLegsNewHeight || ChairsBackGeometry
		.Z != InBackNewHeight || ChairsSpacing != InNewSpacing)
	{
		ChairsSeatGeometry = InSeatNewGeometry;
		ChairsLegsGeometry.Z = InLegsNewHeight;
		ChairsBackGeometry.Z = InBackNewHeight;
		ChairsSpacing = InNewSpacing;
		
		Internal_ComputeAndRegenerateChairs();
		return true;
	}

	return false;
}

bool ATCTable::TrySetGeometryValues(const FVector& InSurfaceData, const FVector& InLegsData)
{
	if (!Internal_ValidateGeometryValues(InSurfaceData, InLegsData))
	{
		return false;
	}

	SurfaceGeometryData = InSurfaceData;
	LegsGeometryData = InLegsData;
	return true;
}

void ATCTable::ClearGeometryData()
{
	Super::ClearGeometryData();
}

void ATCTable::SetSelected_Implementation()
{
	Super::SetSelected_Implementation();

	for(auto& Chair : Chairs)
	{
		if(Chair.IsValid())
		{
			if(Chair->GetClass()->ImplementsInterface(UTCSelectableObjectInterface::StaticClass()))
			{
				Execute_SetSelected(Chair.Get());
			}
		}
	}
}

void ATCTable::SetFocused_Implementation()
{
	Super::SetFocused_Implementation();

	for(auto& Chair : Chairs)
	{
		if(Chair.IsValid())
		{
			if(Chair->GetClass()->ImplementsInterface(UTCSelectableObjectInterface::StaticClass()))
			{
				Execute_SetFocused(Chair.Get());
			}
		}
	}
}

void ATCTable::SetDefault_Implementation()
{
	Super::SetDefault_Implementation();

	for(auto& Chair : Chairs)
	{
		if(Chair.IsValid())
		{
			if(Chair->GetClass()->ImplementsInterface(UTCSelectableObjectInterface::StaticClass()))
			{
				Execute_SetDefault(Chair.Get());
			}
		}
	}
}

bool ATCTable::IsSelected_Implementation() const
{
	return Super::IsSelected_Implementation();
}

bool ATCTable::IsFocused_Implementation() const
{
	return Super::IsFocused_Implementation();
}

bool ATCTable::Internal_ValidateGeometryValues(const FVector& InSurfaceData, const FVector& InLegsData) const
{
	auto ValidateData = [](const FVector& InData)
	{
		return !InData.ContainsNaN() && InData.X > 0 && InData.Y > 0 && InData.Z > 0;
	};

	return ValidateData(InSurfaceData) && ValidateData(InLegsData);
}

void ATCTable::Internal_ComputeChairMaterialSetup(ATCChair* InChair)
{
	check(InChair);
	if(!InChair->GetClass()->ImplementsInterface(UTCSelectableObjectInterface::StaticClass()))
	{
		return;
	}
	
	if(!bIsSelected && !bIsFocused)
	{
		Execute_SetDefault(InChair);
		return;
	}

	if(bIsSelected)
	{
		Execute_SetSelected(InChair);
		return;
	}

	Execute_SetFocused(InChair);
}

void ATCTable::Internal_ComputeAndRegenerateChairs()
{
	int32 ChairsIndex = 0;
	auto ComputeSideChairPosition = [&](const FVector& InChairOffset, float InChairYaw, int32 TableSideSize,
		const FVector& InChairPositionIncremental)
	{
		FVector Position = GetActorLocation() + InChairOffset;
		check(GetWorld());
		for (int i = 0; i < TableSideSize; ++i)
		{
			/*Get a chair from the pool, try generate its geometry and set its position and rotation*/
			bool bReused;
			ATCChair* NewChair = Internal_GetChair(ChairsIndex, bReused);
			if(!NewChair)
			{
				continue;
			}
			NewChair->SetGeometryValues(ChairsSeatGeometry, ChairsLegsGeometry,
				ChairsBackGeometry);
			if (bReused)
			{
				NewChair->TryUpdateRenderedGeometry();
			}
			else
			{
				if (!NewChair->TryRenderGeometry())
				{
					/*If the geometry generation of the new chair is not successful, return it to the pool*/
					ChairPoolComponent->AddChairResource(NewChair);
					continue;
				}
				
				Chairs.Emplace(NewChair);
				Internal_ComputeChairMaterialSetup(NewChair);
			}
			NewChair->SetActorLocation(Position + FVector{ 0, 0,
				NewChair->GetLegsGeometryData().Z + NewChair->GetSurfaceGeometryData().Z / 2 });
			NewChair->SetActorRotation({ 0, InChairYaw, 0 });
			Position += InChairPositionIncremental;
			++ChairsIndex;
		}
	};

	const int ChairsPerLength =
		FMath::TruncToInt(Surface->GetLength() / (ChairsSeatGeometry.Y + ChairsSpacing));
	const int ChairsPerWidth =
		FMath::TruncToInt(Surface->GetWidth() / (ChairsSeatGeometry.Y + ChairsSpacing));

	FVector Offset = { Surface->GetWidth() / 2 + ChairsSeatGeometry.X / 2 + ChairsPaddingFromTable,
		-Surface->GetLength() / 2 + Surface->GetLength() / ChairsPerLength / 2,
		-UpperLeftLeg->GetHeight() - Surface->GetHeight() / 2 };
	FVector PositionIncremental = { 0, Surface->GetLength() / ChairsPerLength, 0 };
	ComputeSideChairPosition(Offset, 0, ChairsPerLength, PositionIncremental);
	Offset.X *= -1;
	ComputeSideChairPosition(Offset, 180, ChairsPerLength, PositionIncremental);

	Offset = { -Surface->GetWidth() / 2 + Surface->GetWidth() / ChairsPerWidth / 2,
		-Surface->GetLength() / 2 - ChairsSeatGeometry.X / 2 - ChairsPaddingFromTable,
		-UpperLeftLeg->GetHeight() - Surface->GetHeight() / 2 };
	PositionIncremental = { (Surface->GetWidth() / ChairsPerWidth), 0, 0 };
	ComputeSideChairPosition(Offset, -90, ChairsPerWidth, PositionIncremental);
	Offset.Y *= -1;
	ComputeSideChairPosition(Offset, 90, ChairsPerWidth, PositionIncremental);

	RemoveUnusedChairs(ChairsIndex);
}

ATCChair* ATCTable::Internal_GetChair(int32 InIndex, bool& bReused)
{
	bReused = false;
	if (Chairs.IsValidIndex(InIndex))
	{
		if (Chairs[InIndex].IsValid())
		{
			bReused = true;
			return Chairs[InIndex].Get();
		}
		return nullptr;
	}
	return ChairPoolComponent->GetChairResource();
}

void ATCTable::RemoveUnusedChairs(int32 InUsedChairsNum)
{
	for (int32 i = Chairs.Num() - 1; i >= InUsedChairsNum; i--)
	{
		ChairPoolComponent->AddChairResource(Chairs[i].Get());
		Chairs.RemoveAt(i, 1);
	}
}
