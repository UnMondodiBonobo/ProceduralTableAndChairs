// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include <Components/Button.h>

#include "Components/TextBlock.h"
#include "ViewModels/TCTableViewModel.h"
#include "TCUITableInstanceSelector.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTableSelectorClickedDelegate, UTCUITableInstanceSelector*, TableSelectorClicked);

UCLASS()
class TABLEANDCHAIRS_API UTCUITableInstanceSelector : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool TrySetTableFocusedState();
	UFUNCTION(BlueprintCallable)
	bool TrySetTableSelectedState();
	UFUNCTION(BlueprintCallable)
	bool TrySetTableDefaultState();
	UFUNCTION(BlueprintCallable)
	void SetViewModel(UTCTableViewModel* InTableViewModel);
	UFUNCTION(BlueprintPure)
	UTCTableViewModel* GetTableViewModel();
	UFUNCTION(BlueprintCallable)
	void SetNumber(int32 InNumber);
	
	UPROPERTY(meta = (ExposeOnSpawn), BlueprintReadOnly)
	int32 StartingNumber = -1;
	UPROPERTY(BlueprintAssignable)
	FOnTableSelectorClickedDelegate OnTableSelectorClicked;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UButton> Button;
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTextBlock> Number;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor SelectedColor {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor UnselectedColor {};
	UPROPERTY(meta = (ExposeOnSpawn), BlueprintReadOnly)
	TWeakObjectPtr<UTCTableViewModel> TableViewModel;

private:
	UFUNCTION()
	void OnButtonClicked();
	void Internal_SetButtonAsSelected();
	void Internal_SetButtonAsUnselected();
};

inline UTCTableViewModel* UTCUITableInstanceSelector::GetTableViewModel()
{
	return TableViewModel.Get();
}

inline void UTCUITableInstanceSelector::SetViewModel(UTCTableViewModel* InTableViewModel)
{
	TableViewModel = InTableViewModel;
}