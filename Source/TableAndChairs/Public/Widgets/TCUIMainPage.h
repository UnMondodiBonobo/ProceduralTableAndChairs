// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>

#include "TCUITableProperty.h"
#include "Widgets/TCUITableInstanceSelector.h"
#include "ViewModels/TCTableViewModel.h"
#include "TCUIMainPage.generated.h"

class UTCUITableInstanceDetailsPanel;
class UScrollBox;

/*Struct used to enable UPROPERTY macro on a kind of TPair*/
USTRUCT(BlueprintType)
struct FTCTableViewModelSelectorsData
{
	GENERATED_BODY()

	FTCTableViewModelSelectorsData()
	{
	}
	
	explicit FTCTableViewModelSelectorsData(UTCTableViewModel* InViewModel)
		: ViewModel(InViewModel)
	{
	}

	FTCTableViewModelSelectorsData(UTCTableViewModel* InViewModel, UTCUITableInstanceSelector* InSelector)
		: ViewModel(InViewModel)
		, Selector(InSelector)
	{
	}
	
	UPROPERTY()
	TWeakObjectPtr<UTCTableViewModel> ViewModel;
	UPROPERTY()
	TObjectPtr<UTCUITableInstanceSelector> Selector;

	bool operator == (const FTCTableViewModelSelectorsData& InOther) const;
};

UCLASS()
class TABLEANDCHAIRS_API UTCUIMainPage : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief Adds the view model with its corresponding selector
	 * @param InTableViewModel Table view model to encapsulate within its corresponding
	 *		  selector in FTCTableViewModelSelectorsData wrapper
	 */
	UFUNCTION(BlueprintCallable)
	void AddViewModel(UTCTableViewModel* InTableViewModel);

	/**
	 * @brief Removes the passed table view model and its corresponding selector
	 * @param InTableViewModel table view model to remove
	 */
	UFUNCTION(BlueprintCallable)
	void RemoveViewModel(UTCTableViewModel* InTableViewModel);

	/*Used to trigger widget animations on blueprints*/
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ToggleTableDetailsPanel();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyOnRenderFailed();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UScrollBox> TableSelectorsScrollBox;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UTCUITableInstanceDetailsPanel> TableDetailsPanel;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTCUITableInstanceSelector> TableSelectorWidgetClass;
	
private:
	UFUNCTION()
	void SetTablePanelViewModel(UTCUITableInstanceSelector* InTableSelectorClicked);
	UFUNCTION()
	void ModifyAllTableRequested(UTCUITableProperty* InProperty, float InValue);
	
	void Internal_SetFocusOnFirstValidSelector(int32 InIndexRemoved);
	void Internal_RemoveFocusedStateFromAllOtherSelectors(UTCUITableInstanceSelector* InIgnoredSelector);
	void Internal_UpdateSelectorsNumbers();

	/*Additional stored struct data ViewModel - Selector to avoid getting the view models, all the time,
	 *from the selectors themselves stored in the scrollbox since they need to be casted every time
	 */
	UPROPERTY()
	TArray<FTCTableViewModelSelectorsData> TableViewModelSelectors;
};
