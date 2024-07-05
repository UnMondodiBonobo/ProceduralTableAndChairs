// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>

#include "ViewModels/TCTableViewModel.h"
#include "TCUITableInstanceDetailsPanel.generated.h"

class UCheckBox;
class UVerticalBox;
class UTCUITableProperty;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnModifyAllTablesRequestedDelegate, UTCUITableProperty*, Property, float, Value);

UCLASS()
class TABLEANDCHAIRS_API UTCUITableInstanceDetailsPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief Utility function that computes the geometry data of a table + chairs given a view model and a property changed.
	 * The property contains a tag that enables the function to discriminate which value has been changed and overwrite
	 * the correct float in the table vectors geometry. The unchanged values will be extracted from the view model.
	 * @param InTableViewModel Table View Model from which to extract the default geometry data
	 * @param InValueChanged Value changed
	 * @param PropertyToEvaluate Property to evaluate, useful for accessing the tag that represents a specific value
	 * in the geometry set of a table
	 * @param OutTableSurface Resulting output table surface
	 * @param OutTableLegs Resulting output table legs
	 * @param OutChairsSeat Resulting output table's chairs seat
	 * @param OutChairsLegs Resulting output table's chairs legs
	 * @param OutChairsBack Resulting output table's chairs back
	 * @param OutChairsSpacing Resulting output table's chairs minimum spacing
	 * @param bTableModified True if the table has been modified, false otherwise
	 * @param bChairsModified True if the chairs have been modified, false otherwise
	 */
	static void ComputeChangedValueInTable(const UTCTableViewModel* InTableViewModel, float InValueChanged,
		const UTCUITableProperty* PropertyToEvaluate, FVector& OutTableSurface, FVector& OutTableLegs,
		FVector& OutChairsSeat, FVector& OutChairsLegs, FVector& OutChairsBack, float& OutChairsSpacing, bool& bTableModified, bool& bChairsModified);
	
	void SetViewModel(UTCTableViewModel* InTableViewModel);

	UPROPERTY(BlueprintAssignable)
	FOnModifyAllTablesRequestedDelegate OnModifyAllTablesRequested;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/*Table Properties and their vertical box*/
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UVerticalBox> TableValues;
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTCUITableProperty> TableWidthProperty;
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTCUITableProperty> TableLengthProperty;
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTCUITableProperty> TableHeightProperty;
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTCUITableProperty> TableLegsHeightProperty;
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadOnly)

	/*Chair Properties and theirs vertical box*/
	TObjectPtr<UVerticalBox> ChairValues;
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTCUITableProperty> ChairWidthProperty;
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTCUITableProperty> ChairLengthProperty;
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTCUITableProperty> ChairHeightProperty;
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTCUITableProperty> ChairLegsHeightProperty;
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTCUITableProperty> ChairBackHeightProperty;
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTCUITableProperty> ChairSpacing;

	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCheckBox> OptimizeCheckBox;
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCheckBox> ModifyAllTablesCheckBox;

private:
	UFUNCTION()
	void OnPropertyValueCommitted(UTCUITableProperty* InProperty, float InValue, ETextCommit::Type InCommitMethod = ETextCommit::Default);
	UFUNCTION()
	void OnPropertySpinBoxBeginMovement(UTCUITableProperty* InProperty);
	UFUNCTION()
	void OnPropertySpinBoxEndMovement(UTCUITableProperty* InProperty, float InValue);
	UFUNCTION()
	void ToggleSliderActivation(bool bBoxChecked);
	UFUNCTION()
	void ModifyAllTablesRequested(bool bBoxChecked);
	
	void Internal_ToggleTableAndChairsValuesChangedBindings(bool bBind);
	void Internal_SetTableViewModelValues();
	void Internal_SetChairsViewModelValues();
	
	TWeakObjectPtr<UTCTableViewModel> TableViewModel;
};
