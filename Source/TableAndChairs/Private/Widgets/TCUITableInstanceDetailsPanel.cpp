// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/TCUITableInstanceDetailsPanel.h"

#include <Components/VerticalBox.h>

#include <Components/CheckBox.h>
#include "Utilities/TCGeometryActorTags.h"
#include "Widgets/TCUITableProperty.h"

void UTCUITableInstanceDetailsPanel::ComputeChangedValueInTable(const UTCTableViewModel* InTableViewModel, float InValueChanged,
	const UTCUITableProperty* PropertyToEvaluate, FVector& OutTableSurface, FVector& OutTableLegs,
	FVector& OutChairsSeat, FVector& OutChairsLegs, FVector& OutChairsBack, float& OutChairsSpacing, bool& bTableModified,
	bool& bChairsModified)
{
	bTableModified = false;
	bChairsModified = false;
	if(!PropertyToEvaluate)
	{
		return;
	}
	if(!InTableViewModel)
	{
		return;
	}

	const UTCGeometryActorTags* GeometryActorTags = GetDefault<UTCGeometryActorTags>();
	check(GeometryActorTags);
	const FGameplayTag& PropertyTag = PropertyToEvaluate->GetTag();
	OutTableSurface = InTableViewModel->GetTableSurfaceGeometryData();
	OutTableLegs = InTableViewModel->GetTableLegsGeometryData();
	OutChairsSeat = InTableViewModel->GetTableChairsSeatGeometryData();
	OutChairsLegs = InTableViewModel->GetTableChairsLegsGeometryData();
	OutChairsBack = InTableViewModel->GetTableChairsBackGeometryData();
	OutChairsSpacing = InTableViewModel->GetTableChairsSpacing();
	InValueChanged *= 100.f;
	
	if(PropertyTag.MatchesTag(GeometryActorTags->TableTagRoot))
	{
		bTableModified = true;
		if(GeometryActorTags->TableWidthTag.MatchesTag(PropertyTag))
		{
			OutTableSurface.Y = InValueChanged;
			return;
		}
		if(GeometryActorTags->TableLengthTag.MatchesTag(PropertyTag))
		{
			OutTableSurface.X = InValueChanged;
			return;
		}
		if(GeometryActorTags->TableHeightTag.MatchesTag(PropertyTag))
		{
			OutTableSurface.Z = InValueChanged;
			return;
		}
		if(GeometryActorTags->TableLegsHeightTag.MatchesTag(PropertyTag))
		{
			OutTableLegs.Z = InValueChanged;
		}
	}
	else if (PropertyTag.MatchesTag(GeometryActorTags->ChairTagRoot))
	{
		/*Chairs cases*/
		bChairsModified = true;
		if(GeometryActorTags->ChairsWidthTag.MatchesTag(PropertyTag))
		{
			OutChairsSeat.Y = InValueChanged;
			return;
		}
		if(GeometryActorTags->ChairsLengthTag.MatchesTag(PropertyTag))
		{
			OutChairsSeat.X = InValueChanged;
			return;
		}
		if(GeometryActorTags->ChairsHeightTag.MatchesTag(PropertyTag))
		{
			OutChairsSeat.Z = InValueChanged;
			return;
		}
		if(GeometryActorTags->ChairsLegsHeightTag.MatchesTag(PropertyTag))
		{
			OutChairsLegs.Z = InValueChanged;
			return;
		}
		if(GeometryActorTags->ChairsBackHeightTag.MatchesTag(PropertyTag))
		{
			OutChairsBack.Z = InValueChanged;
			return;
		}
		if(GeometryActorTags->ChairsSpacingTag.MatchesTag(PropertyTag))
		{
			OutChairsSpacing = InValueChanged;
		}
	}
}

void UTCUITableInstanceDetailsPanel::SetViewModel(UTCTableViewModel* InTableViewModel)
{
	TableViewModel = MakeWeakObjectPtr(InTableViewModel);
	Internal_SetTableViewModelValues();
	Internal_SetChairsViewModelValues();
}

void UTCUITableInstanceDetailsPanel::ModifyAllTablesRequested(bool bBoxChecked)
{
	if(bBoxChecked)
	{
		OptimizeCheckBox->SetIsChecked(bBoxChecked);
		ToggleSliderActivation(bBoxChecked);
	}
}

void UTCUITableInstanceDetailsPanel::NativeConstruct()
{
	Super::NativeConstruct();

	Internal_ToggleTableAndChairsValuesChangedBindings(true);
	ToggleSliderActivation(OptimizeCheckBox->IsChecked());
	OptimizeCheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::ToggleSliderActivation);
	
	/*If modify all tables is checked, we disable the sliders. User can re enable sliders although is not advised for very big numbers or multiple tables*/
	ModifyAllTablesCheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::ModifyAllTablesRequested);
}

void UTCUITableInstanceDetailsPanel::NativeDestruct()
{
	Internal_ToggleTableAndChairsValuesChangedBindings(false);
	OptimizeCheckBox->OnCheckStateChanged.RemoveDynamic(this, &ThisClass::ToggleSliderActivation);
	ModifyAllTablesCheckBox->OnCheckStateChanged.RemoveDynamic(this, &ThisClass::ModifyAllTablesRequested);
	
	Super::NativeDestruct();
}

void UTCUITableInstanceDetailsPanel::OnPropertyValueCommitted(UTCUITableProperty* InProperty, float InValue,
	ETextCommit::Type InCommitMethod)
{
	check(TableViewModel.IsValid());

	FVector TableSurfaceGeometry = FVector::ZeroVector;
	FVector TableLegsGeometry = FVector::ZeroVector;
	FVector ChairsSeatGeometry = FVector::ZeroVector;
	FVector ChairsLegsGeometry = FVector::ZeroVector;
	FVector ChairsBackGeometry = FVector::ZeroVector;
	float ChairsSpacing = 0;
	bool bTableModified = false;
	bool bChairsModified = false;

	ComputeChangedValueInTable(TableViewModel.Get(), InValue, InProperty, TableSurfaceGeometry, TableLegsGeometry,
		ChairsSeatGeometry,ChairsLegsGeometry, ChairsBackGeometry, ChairsSpacing, bTableModified, bChairsModified);

	if (bTableModified)
	{
		TableViewModel->TryUpdateTableGeometry(TableSurfaceGeometry, TableLegsGeometry.Z);
		if(ModifyAllTablesCheckBox->IsChecked())
		{
			OnModifyAllTablesRequested.Broadcast(InProperty, InValue);
		}
		return;
	}

	if (bChairsModified)
	{
		TableViewModel->TryUpdateChairsGeometry(ChairsSeatGeometry, ChairsLegsGeometry.Z, ChairsBackGeometry.Z,
			ChairsSpacing);
		if(ModifyAllTablesCheckBox->IsChecked())
		{
			OnModifyAllTablesRequested.Broadcast(InProperty, InValue);
		}
	}
}

void UTCUITableInstanceDetailsPanel::OnPropertySpinBoxBeginMovement(UTCUITableProperty* InProperty)
{
	check(InProperty);
	InProperty->OnSpinBoxValueChanged.AddDynamic(this, &ThisClass::OnPropertyValueCommitted);
}

void UTCUITableInstanceDetailsPanel::OnPropertySpinBoxEndMovement(UTCUITableProperty* InProperty, float InValue)
{
	check(InProperty);
	InProperty->OnSpinBoxValueChanged.RemoveDynamic(this, &ThisClass::OnPropertyValueCommitted);
}

void UTCUITableInstanceDetailsPanel::Internal_ToggleTableAndChairsValuesChangedBindings(bool bBind)
{
	check(TableValues && ChairValues);
	auto BindToValuesChanged = [this, bBind](TArray<UWidget*> InWidgetProperties)
	{
		for(auto* Widget : InWidgetProperties)
		{
			UTCUITableProperty* Property = Cast<UTCUITableProperty>(Widget);
			if(!Property)
			{
				continue;
			}

			if(bBind)
			{
				Property->OnSpinBoxValueCommitted.AddDynamic(this, &ThisClass::OnPropertyValueCommitted);
				Property->OnSpinBoxBeginMovement.AddDynamic(this, &ThisClass::OnPropertySpinBoxBeginMovement);
				Property->OnSpinBoxEndMovement.AddDynamic(this, &ThisClass::OnPropertySpinBoxEndMovement);
			}
			else
			{
				Property->OnSpinBoxValueCommitted.RemoveDynamic(this, &ThisClass::OnPropertyValueCommitted);
				Property->OnSpinBoxBeginMovement.RemoveDynamic(this, &ThisClass::OnPropertySpinBoxBeginMovement);
				Property->OnSpinBoxEndMovement.RemoveDynamic(this, &ThisClass::OnPropertySpinBoxEndMovement);
			}
		}
	};

	BindToValuesChanged(TableValues->GetAllChildren());
	BindToValuesChanged(ChairValues->GetAllChildren());
}

void UTCUITableInstanceDetailsPanel::Internal_SetTableViewModelValues()
{
	if(!TableViewModel.IsValid())
	{
		return;
	}

	const FVector TableSurface = TableViewModel->GetTableSurfaceGeometryData();
	const float TableLegsZ = TableViewModel->GetTableLegsGeometryData().Z;
	TableWidthProperty->SetPropertyValue(TableSurface.Y);
	TableLengthProperty->SetPropertyValue(TableSurface.X);
	TableHeightProperty->SetPropertyValue(TableSurface.Z);
	TableLegsHeightProperty->SetPropertyValue(TableLegsZ);
}

void UTCUITableInstanceDetailsPanel::Internal_SetChairsViewModelValues()
{
	if(!TableViewModel.IsValid())
	{
		return;
	}

	const FVector ChairsSeat = TableViewModel->GetTableChairsSeatGeometryData();
	const float ChairsLegsZ = TableViewModel->GetTableChairsLegsGeometryData().Z;
	const float ChairsBackZ = TableViewModel->GetTableChairsBackGeometryData().Z;
	const float ChairsSpacing = TableViewModel->GetTableChairsSpacing();
	
	ChairWidthProperty->SetPropertyValue(ChairsSeat.Y);
	ChairLengthProperty->SetPropertyValue(ChairsSeat.X);
	ChairHeightProperty->SetPropertyValue(ChairsSeat.Z);
	ChairLegsHeightProperty->SetPropertyValue(ChairsLegsZ);
	ChairBackHeightProperty->SetPropertyValue(ChairsBackZ);
	ChairSpacing->SetPropertyValue(ChairsSpacing);
}

void UTCUITableInstanceDetailsPanel::ToggleSliderActivation(bool bBoxChecked)
{
	bBoxChecked = !bBoxChecked;
	auto ToggleSliderEnabled = [bBoxChecked](TArray<UWidget*> InWidgetProperties)
	{
		for(auto* Widget : InWidgetProperties)
		{
			UTCUITableProperty* Property = Cast<UTCUITableProperty>(Widget);
			if(!Property)
			{
				continue;
			}

			Property->SetSliderEnabled(bBoxChecked);
		}
	};

	ToggleSliderEnabled(TableValues->GetAllChildren());
	ToggleSliderEnabled(ChairValues->GetAllChildren());
}
