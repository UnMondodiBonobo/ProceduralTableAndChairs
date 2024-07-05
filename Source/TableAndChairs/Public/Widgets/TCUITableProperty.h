// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include <GameplayTagContainer.h>
#include "TCUITableProperty.generated.h"

class USpinBox;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpinBoxSliderBeginMovementDelegate, UTCUITableProperty*, SpinBox);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpinBoxValueChangedDelegate, UTCUITableProperty*, SpinBox, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSpinBoxValueCommittedDelegate, UTCUITableProperty*, SpinBox, float, Value, ETextCommit::Type, CommitMethod);

UCLASS()
class TABLEANDCHAIRS_API UTCUITableProperty : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetPropertyName(const FText& InText);
	UFUNCTION(BlueprintCallable)
	void SetPropertyValue(float InValue);
	UFUNCTION(BlueprintCallable)
	void SetSliderEnabled(bool bInEnabled);
	UFUNCTION(BlueprintPure)
	const FGameplayTag& GetTag() const;

	UPROPERTY(BlueprintAssignable)
	FOnSpinBoxSliderBeginMovementDelegate OnSpinBoxBeginMovement;
	UPROPERTY(BlueprintAssignable)
	FOnSpinBoxValueChangedDelegate OnSpinBoxEndMovement;
	UPROPERTY(BlueprintAssignable)
	FOnSpinBoxValueCommittedDelegate OnSpinBoxValueCommitted;
	UPROPERTY(BlueprintAssignable)
	FOnSpinBoxValueCommittedDelegate OnSpinBoxValueChanged;

	UPROPERTY(EditAnywhere)
	FText PropertyDefaultName {};
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativePreConstruct() override;

	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpinBox> Value;
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTextBlock> PropertyName;
	UPROPERTY(EditAnywhere)
	FGameplayTag ValueTag {};

private:
	UFUNCTION()
	void OnBeginValueMovement();
	UFUNCTION()
	void OnEndValueMovement(float InValue);
	UFUNCTION()
	void OnValueCommitted(float InValue, ETextCommit::Type InCommitMethod);
	UFUNCTION()
	void OnValueChanged(float InValue);
};

inline const FGameplayTag& UTCUITableProperty::GetTag() const
{
	return ValueTag;
}
