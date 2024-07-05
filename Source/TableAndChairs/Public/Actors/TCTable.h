// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "Actors/TCLeggedSurfaceGeometry.h"
#include "TCTable.generated.h"

class UTCChairPoolComponent;
class ATCChair;

UCLASS(Blueprintable, BlueprintType)
class TABLEANDCHAIRS_API ATCTable : public ATCLeggedSurfaceGeometry
{
	GENERATED_BODY()

public:
	ATCTable();
	
	/*See parent class for documentation*/
	virtual bool TryRenderGeometry() override;

	/*See parent class for documentation*/
	virtual bool TryUpdateRenderedGeometry() override;

	/*See parent class for documentation*/
	virtual bool GenerateRandomGeometryValues() override;
	
	/*See parent class for documentation*/
	virtual bool CanRenderGeometryAtLocation(const FVector& InLocationToSpawn,
		TArray<AActor*>& InOptionalActorsToIgnore, const FVector& InBoxExtent = { 0, 0, 0 }) override;

	/**
	 * @brief Try updates chairs geometry with the new given parameters
	 * @param InSeatNewGeometry New seat geometry
	 * @param InLegsNewHeight New legs height geometry
	 * @param InBackNewHeight New back height geometry
	 * @param InNewSpacing New minimum spacing between chairs
	 * @return True if the operation is successful, false otherwise
	 */
	bool TryUpdateChairsGeometry(const FVector& InSeatNewGeometry, const double& InLegsNewHeight,
	const double& InBackNewHeight, const double& InNewSpacing);

	/**
	 * @brief Set the given geometry vectors and cache it if possible, it checks if the geometry passed is > 0
	 * @param InSurfaceData New surface data (surface)
	 * @param InLegsData New legs data (legs)
	 * @return True if the operation is successful, false otherwise
	 */
	bool TrySetGeometryValues(const FVector& InSurfaceData, const FVector& InLegsData);

	virtual void ClearGeometryData() override;
	
	/*ITCSelectableObjectInterface parent functions overrides begin*/
	virtual void SetSelected_Implementation() override;
	virtual void SetFocused_Implementation() override;
	virtual void SetDefault_Implementation() override;
	virtual bool IsSelected_Implementation() const override;
	virtual bool IsFocused_Implementation() const override;
	/*ITCSelectableObjectInterface parent functions overrides end*/

	const FVector& GetChairsSeatGeometryData() const;
	const FVector& GetChairsLegsGeometryData() const;
	const FVector& GetChairsBackGeometryData() const;
	float GetChairsSpacing() const;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTCChairPoolComponent> ChairPoolComponent;
	UPROPERTY(EditAnywhere)
	float ChairsPaddingFromTable = 5.f;

private:
	bool Internal_ValidateGeometryValues(const FVector& InSurfaceData, const FVector& InLegsData) const;
	void Internal_ComputeChairMaterialSetup(ATCChair* InChair);
	void Internal_ComputeAndRegenerateChairs();
	ATCChair* Internal_GetChair(int32 InIndex, bool& bReused);
	void RemoveUnusedChairs(int32 InUsedChairsNum);
	
	TArray<TWeakObjectPtr<ATCChair>> Chairs;
	
	FVector ChairsSeatGeometry {};
	FVector ChairsLegsGeometry {};
	FVector ChairsBackGeometry {};
	float ChairsSpacing = 0;
};

inline const FVector& ATCTable::GetChairsSeatGeometryData() const
{
	return ChairsSeatGeometry;
}

inline const FVector& ATCTable::GetChairsLegsGeometryData() const
{
	return ChairsLegsGeometry;
}

inline const FVector& ATCTable::GetChairsBackGeometryData() const
{
	return ChairsBackGeometry;
}

inline float ATCTable::GetChairsSpacing() const
{
	return ChairsSpacing;
}

