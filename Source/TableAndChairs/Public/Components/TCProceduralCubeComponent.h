// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <ProceduralMeshComponent.h>
#include "TCProceduralCubeComponent.generated.h"

class UTCGeometryDataAsset;
enum class ETCGeometryType : uint8;

UCLASS(BlueprintType, Blueprintable)
class TABLEANDCHAIRS_API UTCProceduralCubeComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief Generate and render the geometry with the given values
	 * @param InWidth Width of the component, X axis 
	 * @param InLength Length of the component, Y axis
	 * @param InHeight Height of the component, Z axis
	 * @return If the operation is successful, false otherwise
	 */
	bool TryGenerateGeometry(const double& InWidth, const double& InLength, const double& InHeight);

	/**
	 * @brief Update the rendered geometry with the new given values
	 * @param InNewWidth New width of the component, X axis
	 * @param InNewLength New Length of the component, Y axis
	 * @param InNewHeight New Height of the component, Z axis
	 * @return If the operation is successful, false otherwise
	 */
	bool TryUpdateGeometry(const double& InNewWidth, const double& InNewLength, const double& InNewHeight);
	virtual void ClearGeometryData();
	
	const double& GetLength() const;
	const double& GetWidth() const;
	const double& GetHeight() const;
	
	TWeakObjectPtr<UMaterialInstanceDynamic> GetAppliedDynamicMaterialInstance();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterial> Material;
	
private:
	bool Internal_UpdateCachedGeometry(const double& InWidth, const double& InLength, const double& InHeight);
	bool Internal_ValidateNewGeometryData(const double& InNewWidth, const double& InNewLength, const double& InNewHeight) const;
	void Internal_GenerateDynamicMaterialInstance();

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstance;
	
	/*Geometry data*/
	TArray<FVector> Vertices;
	
	double Width = 0;
	double Length = 0;
	double Height = 0;
	
	bool bGeometryGenerated = false;
};

inline const double& UTCProceduralCubeComponent::GetLength() const
{
	return Length;
}

inline const double& UTCProceduralCubeComponent::GetWidth() const
{
	return Width;
}

inline const double& UTCProceduralCubeComponent::GetHeight() const
{
	return Height;
}

inline TWeakObjectPtr<UMaterialInstanceDynamic> UTCProceduralCubeComponent::GetAppliedDynamicMaterialInstance()
{
	return MaterialInstance;
}

