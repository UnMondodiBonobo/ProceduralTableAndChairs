// Fill out your copyright notice in the Description page of Project Settings.


#include "ViewModels/TCTableViewModel.h"

#include "Actors/TCTable.h"

void UTCTableViewModel::Initialize(ATCTable* InTable)
{
	if(!InTable)
	{
		return;
	}
	
	Table = MakeWeakObjectPtr(InTable);
	check(Table.IsValid());
}

bool UTCTableViewModel::TryUpdateTableGeometry(const FVector& InSurfaceNewGeometry, const double& InLegsNewHeight)
{
	if(!Table.IsValid())
	{
		return false;
	}

	/*If we can't render this geometry at this location without a collision, early return*/
	const FVector BoxExtentAdditionalPadding = { InSurfaceNewGeometry.X - Table->GetSurfaceGeometryData().X,
		InSurfaceNewGeometry.Y - Table->GetSurfaceGeometryData().Y, 0 };

	TArray<AActor*> OptionalActorsToIgnore;
	if(!Table->CanRenderGeometryAtLocation({0,0,0}, OptionalActorsToIgnore, BoxExtentAdditionalPadding))
	{
		return false;
	}

	/*If the requested geometry values are not valid (< 0), early return*/
	if(!Table->TrySetGeometryValues(InSurfaceNewGeometry, {Table->GetLegsGeometryData().X, Table->GetLegsGeometryData().Y, InLegsNewHeight}))
	{
		return false;
	}

	return Table->TryUpdateRenderedGeometry();
}

bool UTCTableViewModel::TryUpdateChairsGeometry(const FVector& InSeatNewGeometry, const double& InLegsNewHeight,
	const double& InBackNewHeight, const double& InNewSpacing)
{
	if(!Table.IsValid())
	{
		return false;
	}
	
	const FVector BoxExtentAdditionalPadding = { InSeatNewGeometry.X - Table->GetChairsSeatGeometryData().X,
		InSeatNewGeometry.X - Table->GetChairsSeatGeometryData().X, 0 };

	/*If we can't render this geometry at this location without a collision, early return*/
	TArray<AActor*> OptionalActorsToIgnore;
	if(!Table->CanRenderGeometryAtLocation({0,0,0},OptionalActorsToIgnore, BoxExtentAdditionalPadding))
	{
		return false;
	}
	
	return Table->TryUpdateChairsGeometry(InSeatNewGeometry, InLegsNewHeight, InBackNewHeight, InNewSpacing);
}

bool UTCTableViewModel::TrySetTableSelectedState()
{
	if(!Internal_CheckTableInterface())
	{
		return false;
	}

	ITCSelectableObjectInterface::Execute_SetSelected(Table.Get());
	return true;
}

bool UTCTableViewModel::TrySetTableFocusedState()
{
	if(!Internal_CheckTableInterface())
	{
		return false;
	}

	ITCSelectableObjectInterface::Execute_SetFocused(Table.Get());
	return true;
}

bool UTCTableViewModel::TrySetTableDefaultState()
{
	if(!Internal_CheckTableInterface())
	{
		return false;
	}

	ITCSelectableObjectInterface::Execute_SetDefault(Table.Get());
	return true;
}

bool UTCTableViewModel::IsTableSelected() const
{
	if(!Internal_CheckTableInterface())
	{
		return false;
	}

	return ITCSelectableObjectInterface::Execute_IsSelected(Table.Get());
}

bool UTCTableViewModel::IsTableFocused() const
{
	if(!Internal_CheckTableInterface())
	{
		return false;
	}

	return ITCSelectableObjectInterface::Execute_IsFocused(Table.Get());
}

bool UTCTableViewModel::Internal_CheckTableInterface() const
{
	if(!Table.IsValid())
	{
		return false;
	}

	/*Safety guard check, although we know for sure that TCTable implements that UTCSelectableObjectInterface interface*/
	return Table.Get()->GetClass()->ImplementsInterface(UTCSelectableObjectInterface::StaticClass());
}
