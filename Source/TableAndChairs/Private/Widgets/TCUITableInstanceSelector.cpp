// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TCUITableInstanceSelector.h"

#include <Components/Button.h>
#include "ViewModels/TCTableViewModel.h"

bool UTCUITableInstanceSelector::TrySetTableFocusedState()
{
	if(!TableViewModel.IsValid())
	{
		return false;
	}

	Internal_SetButtonAsSelected();
	return TableViewModel.Get()->TrySetTableFocusedState();
}

bool UTCUITableInstanceSelector::TrySetTableSelectedState()
{
	if(!TableViewModel.IsValid())
	{
		return false;
	}
	
	Internal_SetButtonAsUnselected();
	return TableViewModel.Get()->TrySetTableSelectedState();
}

bool UTCUITableInstanceSelector::TrySetTableDefaultState()
{
	if(!TableViewModel.IsValid())
	{
		return false;
	}

	Internal_SetButtonAsUnselected();
	return TableViewModel.Get()->TrySetTableDefaultState();
}

void UTCUITableInstanceSelector::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.AddDynamic(this, &ThisClass::OnButtonClicked);
	SetNumber(StartingNumber);
}

void UTCUITableInstanceSelector::NativeDestruct()
{
	Button->OnClicked.RemoveDynamic(this, &ThisClass::OnButtonClicked);
	TableViewModel.Reset();
	
	Super::NativeDestruct();
}

void UTCUITableInstanceSelector::OnButtonClicked()
{
	TrySetTableFocusedState();
	OnTableSelectorClicked.Broadcast(this);
}

void UTCUITableInstanceSelector::Internal_SetButtonAsSelected()
{
	/*Since Button is BindWidget, it must be valid or the Blueprint will not compile*/
	check(Button);
	Button->SetBackgroundColor(SelectedColor);
}

void UTCUITableInstanceSelector::Internal_SetButtonAsUnselected()
{
	/*Since Button is BindWidget, it must be valid or the Blueprint will not compile*/
	check(Button);
	Button->SetBackgroundColor(UnselectedColor);
}

void UTCUITableInstanceSelector::SetNumber(int32 InNumber)
{
	/*Since Number is BindWidget, it must be valid or the Blueprint will not compile*/
	check(Number);
	Number->SetText(FText::AsNumber(InNumber));
}
