// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TCProceduralCubeComponent.h"

#include "Kismet/KismetMaterialLibrary.h"
#include "Utilities/TCGeometrySpawner.h"

bool UTCProceduralCubeComponent::Internal_UpdateCachedGeometry(const double& InWidth, const double& InLength, const double& InHeight)
{
	/*If the new data passed in are identical to the last we cached it is useless to update the geometry*/
	if(!Internal_ValidateNewGeometryData(InWidth, InLength, InHeight))
	{
		return false;
	}

	Width = InWidth;
	Length = InLength;
	Height = InHeight;
	
	Vertices = {
		/*Front*/
		{ Width / 2,-Length / 2, Height / 2}, //0
		{ Width / 2, Length / 2, Height / 2}, //1
		{-Width / 2, Length / 2, Height / 2}, //2
		{-Width / 2,-Length / 2, Height / 2}, //3
		/*Back*/
		{ Width / 2,-Length / 2,-Height / 2}, //4
		{ Width / 2, Length / 2,-Height / 2}, //5
		{-Width / 2, Length / 2,-Height / 2}, //6
		{-Width / 2,-Length / 2,-Height / 2}, //7
		/*Right*/                          
		{ Width / 2, Length / 2,-Height / 2}, //8
		{ Width / 2, Length / 2, Height / 2}, //9
		{ Width / 2,-Length / 2, Height / 2}, //10
		{ Width / 2,-Length / 2,-Height / 2}, //11
		/*Up*/                                
		{-Width / 2, Length / 2,-Height / 2}, //12
		{-Width / 2, Length / 2, Height / 2}, //13
		{ Width / 2, Length / 2, Height / 2}, //14
		{ Width / 2, Length / 2,-Height / 2}, //15
		/*Left*/                              
		{-Width / 2,-Length / 2,-Height / 2}, //16
		{-Width / 2,-Length / 2, Height / 2}, //17
		{-Width / 2, Length / 2, Height / 2}, //18
		{-Width / 2, Length / 2,-Height / 2}, //19
		/*Down*/                              
		{ Width / 2,-Length / 2,-Height / 2}, //20
		{ Width / 2,-Length / 2, Height / 2}, //21
		{-Width / 2,-Length / 2, Height / 2}, //22
		{-Width / 2,-Length / 2,-Height / 2}, //23
	};
	
	return true;
}

bool UTCProceduralCubeComponent::Internal_ValidateNewGeometryData(const double& InNewWidth, const double& InNewLength,
	const double& InNewHeight) const
{
	return InNewWidth > 0 && InNewLength > 0 && InNewHeight > 0;
}

bool UTCProceduralCubeComponent::TryGenerateGeometry(const double& InWidth, const double& InLength, const double& InHeight)
{
	/*If this mesh is already generated, don't create it again, use TryUpdateGeometry since it is more efficient*/
	if(bGeometryGenerated)
	{
		check(GetOwner());
		#if WITH_EDITOR
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
				FString::Printf(
					TEXT(
						"Tried to re generate geometry of already generated component %s on %s, call TryUpdateGeometry() instead"),
					*GetName(), *GetOwner()->GetName()));
		}
		#endif
		return false;
	}
	
	if(!Internal_UpdateCachedGeometry(InWidth, InLength, InHeight))
	{
		return false;
	}

	CreateMeshSection(0, Vertices, ATCGeometrySpawner::CubeGeometryDefaultTriangles,
	                  ATCGeometrySpawner::CubeGeometryDefaultNormals, ATCGeometrySpawner::CubeGeometryDefaultUV0,
	                  TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	Internal_GenerateDynamicMaterialInstance();
	if(MaterialInstance)
	{
		SetMaterial(0,MaterialInstance);
	}

	bGeometryGenerated = true;
	return bGeometryGenerated;
}

bool UTCProceduralCubeComponent::TryUpdateGeometry(const double& InNewWidth, const double& InNewLength,
	const double& InNewHeight)
{
	if(!Internal_UpdateCachedGeometry(InNewWidth, InNewLength, InNewHeight))
	{
		return false;
	}

	/*Update the mesh section with the set geometry, this is faster than Create Mesh Section*/
	UpdateMeshSection(0, Vertices, ATCGeometrySpawner::CubeGeometryDefaultNormals,
	                  ATCGeometrySpawner::CubeGeometryDefaultUV0, TArray<FColor>(), TArray<FProcMeshTangent>());
	
	if(MaterialInstance)
	{
		SetMaterial(0,MaterialInstance);
	}

	return true;
}

void UTCProceduralCubeComponent::ClearGeometryData()
{
	bGeometryGenerated = false;
	Vertices.Empty();
	Width = 0;
	Length = 0;
	Height = 0;
	MaterialInstance = nullptr;
	ClearMeshSection(0);
}

void UTCProceduralCubeComponent::Internal_GenerateDynamicMaterialInstance()
{
	if(!Material)
	{
		return;
	}
	
	MaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, Material);
}
