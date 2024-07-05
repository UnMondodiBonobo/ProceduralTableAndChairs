// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TCUIMainPage.h"

#include <Components/ScrollBox.h>
#include "Widgets/TCUITableInstanceDetailsPanel.h"

bool FTCTableViewModelSelectorsData::operator==(const FTCTableViewModelSelectorsData& InOther) const
{
	return ViewModel.IsValid() && InOther.ViewModel.IsValid() && ViewModel == InOther.ViewModel;
}

void UTCUIMainPage::AddViewModel(UTCTableViewModel* InTableViewModel)
{
	/*Since Table details panel is bind widget, we can check it*/
	check(TableDetailsPanel);
	if (!InTableViewModel)
	{
		return;
	}

	if (TableViewModelSelectors.IsEmpty())
	{
		/*If this is the first view model added, we need to trigger the animations of the details panel and show it on screen*/
		BP_ToggleTableDetailsPanel();
	}

	/*Contains by predicate avoids us to create another useless struct just to invoke operator == on FTCTableViewModelSelector structs*/
	if (TableViewModelSelectors.ContainsByPredicate(
		[InTableViewModel](const FTCTableViewModelSelectorsData& TableData)
		{
			return TableData.ViewModel == InTableViewModel;
		}))
	{
		return;
	}

	if (!TableSelectorWidgetClass)
	{
		#if WITH_EDITOR
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
				FString::Printf(
					TEXT("Table selector widget class not selected in %s, insert a valid Table selector widget class "),
					*GetName()));
		}
			#endif
		return;
	}

	const int32 SelectorNum = TableSelectorsScrollBox->GetChildrenCount() + 1;
	UTCUITableInstanceSelector* NewSelector = CreateWidget<UTCUITableInstanceSelector>(GetWorld(),
		TableSelectorWidgetClass);
	check(NewSelector);

	/*Initialize selector data and bind to its clicked event*/
	TableViewModelSelectors.Emplace(InTableViewModel, NewSelector);
	TableSelectorsScrollBox->AddChild(NewSelector);
	NewSelector->SetViewModel(InTableViewModel);
	NewSelector->SetNumber(SelectorNum);
	NewSelector->OnTableSelectorClicked.AddDynamic(this, &ThisClass::SetTablePanelViewModel);
	SetTablePanelViewModel(NewSelector);
}

void UTCUIMainPage::RemoveViewModel(UTCTableViewModel* InTableViewModel)
{
	check(TableDetailsPanel);
	if (!InTableViewModel)
	{
		return;
	}

	/*Contains by predicate avoids us to create another useless struct just to invoke operator == on FTCTableViewModelSelector structs*/
	FTCTableViewModelSelectorsData* TableViewModelDataFound = TableViewModelSelectors.FindByPredicate(
		[InTableViewModel](const FTCTableViewModelSelectorsData& TableData)
		{
			return TableData.ViewModel == InTableViewModel;
		});

	if (!TableViewModelDataFound)
	{
		return;
	}

	const int32 SelectorIndex = TableSelectorsScrollBox->GetChildIndex(TableViewModelDataFound->Selector);
	/*If for any reason we did not find the widget in the scroll box, we should return*/
	if (SelectorIndex == INDEX_NONE)
	{
		return;
	}

	if (TableViewModelDataFound->ViewModel->IsTableFocused())
	{
		Internal_SetFocusOnFirstValidSelector(SelectorIndex);
	}

	if (!TableViewModelDataFound->ViewModel->TrySetTableDefaultState())
	{
		return;
	}

	check(TableViewModelDataFound->Selector);
	TableViewModelDataFound->Selector->OnTableSelectorClicked.RemoveDynamic(this,
		&ThisClass::SetTablePanelViewModel);
	TableViewModelSelectors.RemoveSingle(*TableViewModelDataFound);
	TableSelectorsScrollBox->RemoveChildAt(SelectorIndex);

	/*If the is no child left, trigger the table details panel hide animation*/
	if (TableSelectorsScrollBox->GetChildrenCount() == 0 && TableViewModelSelectors.IsEmpty())
	{
		BP_ToggleTableDetailsPanel();
		return;
	}

	Internal_UpdateSelectorsNumbers();
}

void UTCUIMainPage::NativeConstruct()
{
	Super::NativeConstruct();

	TableDetailsPanel->OnModifyAllTablesRequested.AddDynamic(this, &ThisClass::ModifyAllTableRequested);
}

void UTCUIMainPage::NativeDestruct()
{
	TableDetailsPanel->OnModifyAllTablesRequested.RemoveDynamic(this, &ThisClass::ModifyAllTableRequested);

	Super::NativeDestruct();
}

void UTCUIMainPage::SetTablePanelViewModel(UTCUITableInstanceSelector* InTableSelectorClicked)
{
	check(InTableSelectorClicked && InTableSelectorClicked->GetTableViewModel());
	if(!InTableSelectorClicked->TrySetTableFocusedState())
	{
		return;
	}
	
	TableDetailsPanel->SetViewModel(InTableSelectorClicked->GetTableViewModel());
	Internal_RemoveFocusedStateFromAllOtherSelectors(InTableSelectorClicked);
}

void UTCUIMainPage::ModifyAllTableRequested(UTCUITableProperty* InProperty, float InValue)
{
	FVector TableSurfaceGeometry = FVector::ZeroVector;
	FVector TableLegsGeometry = FVector::ZeroVector;
	FVector ChairsSeatGeometry = FVector::ZeroVector;
	FVector ChairsLegsGeometry = FVector::ZeroVector;
	FVector ChairsBackGeometry = FVector::ZeroVector;
	float ChairsSpacing = 0;
	bool bTableModified = false;
	bool bChairsModified = false;

	for (auto& TableViewModelData : TableViewModelSelectors)
	{
		if (!TableViewModelData.ViewModel.IsValid())
		{
			continue;
		}

		UTCUITableInstanceDetailsPanel::ComputeChangedValueInTable(TableViewModelData.ViewModel.Get(), InValue,
			InProperty, TableSurfaceGeometry, TableLegsGeometry,
			ChairsSeatGeometry, ChairsLegsGeometry, ChairsBackGeometry, ChairsSpacing, bTableModified,
			bChairsModified);

		if (bTableModified)
		{
			TableViewModelData.ViewModel->TryUpdateTableGeometry(TableSurfaceGeometry, TableLegsGeometry.Z);
		}

		if (bChairsModified)
		{
			TableViewModelData.ViewModel->TryUpdateChairsGeometry(ChairsSeatGeometry, ChairsLegsGeometry.Z,
				ChairsBackGeometry.Z,
				ChairsSpacing);
		}

		bTableModified = false;
		bChairsModified = false;
	}
}

void UTCUIMainPage::Internal_SetFocusOnFirstValidSelector(int32 InIndexRemoved)
{
	if(TableSelectorsScrollBox->GetChildrenCount() == 0)
	{
		/*if the scroll box is empty, we cannot focus any other selector*/
		return;
	}
	
	/*Try to set first the focus on the next selector after the removed one*/
	UWidget* FirstValidWidget = TableSelectorsScrollBox->GetChildAt(InIndexRemoved + 1);
	if(!FirstValidWidget)
	{
		/*If the next one is not valid, try to set first the focus on the previous selector before the removed one*/
		FirstValidWidget = TableSelectorsScrollBox->GetChildAt(InIndexRemoved - 1);
		if(!FirstValidWidget)
		{
			return;
		}
	}
	
	UTCUITableInstanceSelector* FirstValidSelector = Cast<UTCUITableInstanceSelector>(FirstValidWidget);
	check(FirstValidSelector);
	SetTablePanelViewModel(FirstValidSelector);
	Internal_RemoveFocusedStateFromAllOtherSelectors(FirstValidSelector);
}

void UTCUIMainPage::Internal_RemoveFocusedStateFromAllOtherSelectors(UTCUITableInstanceSelector* InIgnoredSelector)
{
	TArray<UWidget*> Selectors = TableSelectorsScrollBox->GetAllChildren();
	if(Selectors.IsEmpty())
	{
		return;
	}

	/*Loop all scroll box selectors and set selected state to all of them (if were selected or focused) but the ignored one*/
	for(auto* Selector : Selectors)
	{
		UTCUITableInstanceSelector* CastedSelector = Cast<UTCUITableInstanceSelector>(Selector);
		check(CastedSelector);
		if(CastedSelector == InIgnoredSelector)
		{
			continue;
		}

		if(CastedSelector->GetTableViewModel()->IsTableSelected() || CastedSelector->GetTableViewModel()->IsTableFocused())
		{
			CastedSelector->TrySetTableSelectedState();
		}
	}
}

void UTCUIMainPage::Internal_UpdateSelectorsNumbers()
{
	TArray<UWidget*> Selectors = TableSelectorsScrollBox->GetAllChildren();
	if(Selectors.IsEmpty())
	{
		return;
	}

	int32 NewIndex = 0;
	/*Loop all scroll box selectors and set selected state to all of them but the ignored one*/
	for(auto* Selector : Selectors)
	{
		UTCUITableInstanceSelector* CastedSelector = Cast<UTCUITableInstanceSelector>(Selector);
		check(CastedSelector);
		CastedSelector->SetNumber(++NewIndex);
	}
}

