// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "TCLeggedSurfaceGeometry.h"
#include <GameFramework/Actor.h>
#include "TCChair.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TABLEANDCHAIRS_API ATCChair : public ATCLeggedSurfaceGeometry
{
	GENERATED_BODY()
	
public:	
	ATCChair();
	/*See parent class for documentation*/
	virtual bool TryRenderGeometry() override;
	
	/*See parent class for documentation*/
	virtual bool TryUpdateRenderedGeometry() override;

	/**
	 * @brief Set the given geometry vectors and cache it if possible, it checks if the geometry passed is > 0
	 * @param InSeatData New Seat data (surface)
	 * @param InLegsData New Legs data (legs)
	 * @param InBackData New back data (back)
	 * @return True if the operation is successful, false otherwise
	 */
	bool SetGeometryValues(const FVector& InSeatData, const FVector& InLegsData,
		const FVector& InBackData);

	virtual void ClearGeometryData() override;
	
	/*ITCSelectableObjectInterface parent functions overrides begin*/
	virtual void SetSelected_Implementation() override;
	virtual void SetFocused_Implementation() override;
	virtual void SetDefault_Implementation() override;
	/*ITCSelectableObjectInterface parent functions overrides end*/
	
	const FVector& GetBackGeometry() const;

protected:
	virtual void UpdateComponentsLocations() override;

private:
	bool Internal_ValidateGeometryValues(const FVector& InSurfaceData, const FVector& InLegsData, const FVector& InBackData);
	void Internal_ApplyDynamicMaterialInfo(const FTCSelectableObjectInfoData& InMaterialInfo);
	
	/*TObjectPtr is already initialized to nullptr by default*/
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTCProceduralCubeComponent> Back;

	FVector BackGeometryData = {};
};

inline const FVector& ATCChair::GetBackGeometry() const
{
	return BackGeometryData;
}

