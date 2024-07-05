// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TCChair.h"
#include "Components/TCProceduralCubeComponent.h"
#include "Components/TCSelectableObjectInfoComponent.h"

/*Sets default values*/
ATCChair::ATCChair()
{
	Back = CreateDefaultSubobject<UTCProceduralCubeComponent>("Back");
	check(Back);
	Back->AttachToComponent(Surface, FAttachmentTransformRules::KeepRelativeTransform);
}

bool ATCChair::SetGeometryValues(const FVector& InSeatData, const FVector& InLegsData,
	const FVector& InBackData)
{
	if(!Internal_ValidateGeometryValues(InSeatData, InLegsData, InBackData))
	{
		return false;
	}

	SurfaceGeometryData = InSeatData;
	LegsGeometryData = InLegsData;
	BackGeometryData = InBackData;
	return true;
}

void ATCChair::ClearGeometryData()
{
	Super::ClearGeometryData();

	Back->ClearGeometryData();
}

void ATCChair::SetSelected_Implementation()
{
	Super::SetSelected_Implementation();
	Internal_ApplyDynamicMaterialInfo(SelectableInfoComponent->GetSelectedInfo());
}

void ATCChair::SetFocused_Implementation()
{
	Super::SetFocused_Implementation();
	Internal_ApplyDynamicMaterialInfo(SelectableInfoComponent->GetFocusedInfo());
}

void ATCChair::SetDefault_Implementation()
{
	Super::SetDefault_Implementation();
	Internal_ApplyDynamicMaterialInfo(SelectableInfoComponent->GetDefaultInfo());
}

bool ATCChair::Internal_ValidateGeometryValues(const FVector& InSurfaceData, const FVector& InLegsData,
	const FVector& InBackData)
{
	auto ValidateData = [](const FVector& InData)
	{
		return !InData.ContainsNaN() && InData.X > 0 && InData.Y > 0 && InData.Z > 0;
	};
	
	return ValidateData(InSurfaceData) && ValidateData(InLegsData) && ValidateData(InBackData);
}

void ATCChair::Internal_ApplyDynamicMaterialInfo(const FTCSelectableObjectInfoData& InMaterialInfo)
{
	const TWeakObjectPtr<UMaterialInstanceDynamic>& BackMaterialInstance = Back->GetAppliedDynamicMaterialInstance();
	if(!BackMaterialInstance.IsValid())
	{
		return;
	}

	BackMaterialInstance->SetVectorParameterValue(InMaterialInfo.ColorParamName, InMaterialInfo.Color);
	BackMaterialInstance->SetScalarParameterValue(InMaterialInfo.EmissiveValueParamName, InMaterialInfo.EmissiveColorValue);
}

bool ATCChair::TryRenderGeometry()
{
	if(!Back->TryGenerateGeometry(BackGeometryData.X, BackGeometryData.Y, BackGeometryData.Z))
	{
		return false;
	}
	
	if(!Super::TryRenderGeometry())
	{
		return false;
	}
	
	return true;
}

bool ATCChair::TryUpdateRenderedGeometry()
{
	if(!Back->TryUpdateGeometry(BackGeometryData.X, BackGeometryData.Y, BackGeometryData.Z))
	{
		return false;
	}
	
	if (!Super::TryUpdateRenderedGeometry())
	{
		return false;
	}
	
	return true;
}

void ATCChair::UpdateComponentsLocations()
{
	Super::UpdateComponentsLocations();
	Back->SetRelativeLocation({ Surface->GetWidth() / 2 - Back->GetWidth() / 2, 0,
		Back->GetHeight() / 2 + Surface->GetHeight() / 2 });
}

