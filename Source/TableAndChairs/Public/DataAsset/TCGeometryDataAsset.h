// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Engine/DataAsset.h>
#include "TCGeometryDataAsset.generated.h"

/**
 * Geometry data asset useful for any legged surface random generated values.
 */
UCLASS(BlueprintType, Blueprintable)
class TABLEANDCHAIRS_API UTCGeometryDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FVector2D SurfaceXAxisMinMaxValues;
	UPROPERTY(EditAnywhere)
	FVector2D SurfaceYAxisMinMaxValues;
	UPROPERTY(EditAnywhere)
	FVector2D SurfaceZAxisMinMaxValues;
	UPROPERTY(EditAnywhere)
	FVector2D LegsXAxisMinMaxValues;
	UPROPERTY(EditAnywhere)
	FVector2D LegsYAxisMinMaxValues;
	UPROPERTY(EditAnywhere)
	FVector2D LegsZAxisMinMaxValues;
	
	virtual bool IsValid() const;
};

inline bool UTCGeometryDataAsset::IsValid() const
{
	auto Validate = [](const FVector2D& InData)
	{
		return InData.X > 0 && InData.Y > 0;
	};

	return Validate(SurfaceXAxisMinMaxValues) &&
		Validate(SurfaceYAxisMinMaxValues) &&
		Validate(SurfaceZAxisMinMaxValues) &&
		Validate(LegsXAxisMinMaxValues) &&
		Validate(LegsYAxisMinMaxValues) &&
		Validate(LegsZAxisMinMaxValues);
}
