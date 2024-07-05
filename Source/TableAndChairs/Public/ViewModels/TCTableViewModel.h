// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <MVVMViewModelBase.h>

#include "Actors/TCTable.h"
#include "TCTableViewModel.generated.h"

class ATCTable;

UCLASS()
class TABLEANDCHAIRS_API UTCTableViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	/**
	 * @brief Stores a table data and cache its data for future use, also binds itself to delegates
	 * @param InTable Table data parameter that this ViewModel will be bound to
	 */
	UFUNCTION(BlueprintCallable)
	void Initialize(ATCTable* InTable);

	/**
	 * @brief Request a table's rendered geometry update, calls CanRenderGeometry and TryUpdateRenderedGeometry
	 * on a cached table, if any is set
	 * @param InSurfaceNewGeometry New surface geometry to give to the table
	 * @param InLegsNewHeight New legs height to give to the table
	 * @return True if the operation is successful, false otherwise
	 */
	UFUNCTION(BlueprintCallable)
	bool TryUpdateTableGeometry(const FVector& InSurfaceNewGeometry, const double& InLegsNewHeight);

	/**
	 * @brief Request a table's chairs rendered geometry update, calls CanRenderGeometry and TryUpdateRenderedGeometry
	 * on a cached table, if any is set
	 * @param InSeatNewGeometry New table's chairs seat geometry
	 * @param InLegsNewHeight New table's chairs legs height geometry
	 * @param InBackNewHeight New table's chairs backs height geometry
	 * @param InNewSpacing New table's chairs minimum spacing
	 * @return True if the operation is successful, false otherwise
	 */
	UFUNCTION(BlueprintCallable)
	bool TryUpdateChairsGeometry(const FVector& InSeatNewGeometry, const double& InLegsNewHeight,
		const double& InBackNewHeight, const double& InNewSpacing);

	/*Table requests to SelectableObjectInterface overrides BEGIN*/
	UFUNCTION(BlueprintCallable)
	bool TrySetTableSelectedState();
	UFUNCTION(BlueprintCallable)
	bool TrySetTableFocusedState();
	UFUNCTION(BlueprintCallable)
	bool TrySetTableDefaultState();
	UFUNCTION(BlueprintPure)
	bool IsTableSelected() const;
	UFUNCTION(BlueprintPure)
	bool IsTableFocused() const;
	/*Table requests to SelectableObjectInterface overrides END*/
	
	UFUNCTION(BlueprintPure)
	FVector GetTableSurfaceGeometryData() const;
	UFUNCTION(BlueprintPure)
	FVector GetTableLegsGeometryData() const;
	UFUNCTION(BlueprintPure)
	FVector GetTableChairsSeatGeometryData() const;
	UFUNCTION(BlueprintPure)
	FVector GetTableChairsLegsGeometryData() const;
	UFUNCTION(BlueprintPure)
	FVector GetTableChairsBackGeometryData() const;
	UFUNCTION(BlueprintPure)
	float GetTableChairsSpacing() const;
	
private:
	bool Internal_CheckTableInterface() const;
	
	TWeakObjectPtr<ATCTable> Table;
};

inline FVector UTCTableViewModel::GetTableSurfaceGeometryData() const
{
	return Table.IsValid() ? Table->GetSurfaceGeometryData() : FVector{};
}

inline FVector UTCTableViewModel::GetTableLegsGeometryData() const
{
	return Table.IsValid() ? Table->GetLegsGeometryData() : FVector{};
}

inline FVector UTCTableViewModel::GetTableChairsSeatGeometryData() const
{
	return Table.IsValid() ? Table->GetChairsSeatGeometryData() : FVector{};
}

inline FVector UTCTableViewModel::GetTableChairsLegsGeometryData() const
{
	return Table.IsValid() ? Table->GetChairsLegsGeometryData() : FVector{};
}

inline FVector UTCTableViewModel::GetTableChairsBackGeometryData() const
{
	return Table.IsValid() ? Table->GetChairsBackGeometryData() : FVector{};
}

inline float UTCTableViewModel::GetTableChairsSpacing() const
{
	return Table.IsValid() ? Table->GetChairsSpacing() : -1;
}