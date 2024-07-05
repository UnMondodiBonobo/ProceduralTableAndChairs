// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "DataAsset/TCGeometryDataAsset.h"
#include "TCChairGeometryDataAsset.generated.h"

/**
 * Geometry data asset useful for chairs random generated values, inherits from GeometryDataAsset
 */
UCLASS(Blueprintable, BlueprintType)
class TABLEANDCHAIRS_API UTCChairGeometryDataAsset : public UTCGeometryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FVector2D BackXAxisMinMaxValues;
	UPROPERTY(EditAnywhere)
	FVector2D BackYAxisMinMaxValues;
	UPROPERTY(EditAnywhere)
	FVector2D BackZAxisMinMaxValues;
	
	/*The spacing, although not advised, could be 0, so it's not mandatory for the validation*/
	UPROPERTY(EditAnywhere)
	FVector2D Spacing;

	UFUNCTION(BlueprintPure)
	virtual bool IsValid() const override;
};

inline bool UTCChairGeometryDataAsset::IsValid() const
{
	if(!Super::IsValid())
	{
		return false;
	}
	
	auto Validate = [](const FVector2D& InData)
	{
		return InData.X > 0 && InData.Y > 0;
	};

	return Validate(BackXAxisMinMaxValues) &&
		Validate(BackYAxisMinMaxValues) &&
		Validate(BackZAxisMinMaxValues);
}