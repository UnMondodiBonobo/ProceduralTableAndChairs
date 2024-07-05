// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "Interfaces/TCSelectableObjectInterface.h"
#include "TCLeggedSurfaceGeometry.generated.h"

struct FTCSelectableObjectInfoData;
class UTCSelectableObjectInfoComponent;
class UTCGeometryDataAsset;
class UTCProceduralCubeComponent;

UCLASS(NotBlueprintable, Abstract)
class TABLEANDCHAIRS_API ATCLeggedSurfaceGeometry : public AActor, public ITCSelectableObjectInterface
{
	GENERATED_BODY()
	
public:	
	ATCLeggedSurfaceGeometry();

	/**
	 * @brief Try render geometry with the cached Geometry Values, CreateMeshSection will be called.
	 * If the geometry of any of the components is already generated, the operation will not be successful
	 * @return True if the operation is successful, false otherwise
	 */
	virtual bool TryRenderGeometry();

	/**
	 * @brief Try update rendered geometry with the cached Geometry Values, UpdateMeshSection will be called.
	 * @return True if the operation is successful, false otherwise
	 */
	virtual bool TryUpdateRenderedGeometry();

	/**
	 * @brief Generates random values from the given geometry data asset
	 * @return True if the operation is successful, false otherwise
	 */
	virtual bool GenerateRandomGeometryValues();

	/**
	 * @brief Check if can render the cached geometry + BoxExtent geometry without colliding
	 * with anything else other then the optional ignored actors passed as parameters 
	 * @param InLocationToSpawn Location where to spawn the box and project the geometry
	 * @param InOptionalActorsToIgnore Actors to ignore from the collision check
	 * @param InBoxExtent Optional extent to add to the geometry cached values
	 * @return True if the operation is successful, false otherwise
	 */
	virtual bool CanRenderGeometryAtLocation(const FVector& InLocationToSpawn,
		 TArray<AActor*>& InOptionalActorsToIgnore, const FVector& InBoxExtent = {0,0,0});

	virtual void ClearGeometryData();
	
	/*ITCSelectableObjectInterface functions implementations begin*/
	virtual void SetSelected_Implementation() override;
	virtual void SetFocused_Implementation() override;
	virtual void SetDefault_Implementation() override;
	virtual bool IsSelected_Implementation() const override;
	virtual bool IsFocused_Implementation() const override;
	/*ITCSelectableObjectInterface functions implementations end*/
	
	const FVector& GetSurfaceGeometryData() const;
	const FVector& GetLegsGeometryData() const;
	const FGuid& GetId() const;

protected:
	virtual void UpdateComponentsLocations();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTCGeometryDataAsset> GeometryDataAsset;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTCProceduralCubeComponent> Surface;
	UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UTCProceduralCubeComponent> UpperLeftLeg;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTCProceduralCubeComponent> DownLeftLeg;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTCProceduralCubeComponent> UpperRightLeg;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTCProceduralCubeComponent> DownRightLeg;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTCSelectableObjectInfoComponent> SelectableInfoComponent;

	UPROPERTY()
	TArray<TObjectPtr<UTCProceduralCubeComponent>> Legs;

	FVector SurfaceGeometryData {};
	FVector LegsGeometryData {};
	
private:
	void Internal_ApplyDynamicMaterialInfo(const FTCSelectableObjectInfoData& InMaterialInfo);
	
	FGuid Id {};
};

inline bool ATCLeggedSurfaceGeometry::IsSelected_Implementation() const
{
	return bIsSelected;
}

inline bool ATCLeggedSurfaceGeometry::IsFocused_Implementation() const
{
	return bIsFocused;
}

inline const FVector& ATCLeggedSurfaceGeometry::GetSurfaceGeometryData() const
{
	return SurfaceGeometryData;
}

inline const FVector& ATCLeggedSurfaceGeometry::GetLegsGeometryData() const
{
	return LegsGeometryData;
}

inline const FGuid& ATCLeggedSurfaceGeometry::GetId() const
{
	return Id;
}

