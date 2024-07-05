// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TCUITableProperty.h"
#include <Components/SpinBox.h>
#include <Components/TextBlock.h>

void UTCUITableProperty::SetPropertyName(const FText& InText)
{
	/*Since this is called on pre construct and we could be in a situation in which this TextBlock could be absent, don't check it*/
	if(!PropertyName)
	{
		return;
	}
	
	PropertyName->SetText(InText);
}

void UTCUITableProperty::SetPropertyValue(float InValue)
{
	/*Since Value is BindWidget, it must be valid or the Blueprint will not compile*/
	check(Value);
	Value->SetValue(InValue / 100.f);
}

void UTCUITableProperty::SetSliderEnabled(bool bInEnabled)
{
	/*Since Value is BindWidget, it must be valid or the Blueprint will not compile*/
	check(Value);
	Value->SetEnableSlider(bInEnabled);
}

void UTCUITableProperty::NativeConstruct()
{
	Super::NativeConstruct();

	/*Since Value is BindWidget, it must be valid or the Blueprint will not compile*/
	check(Value);
	Value->OnBeginSliderMovement.AddDynamic(this, &ThisClass::OnBeginValueMovement);
	Value->OnEndSliderMovement.AddDynamic(this, &ThisClass::OnEndValueMovement);
	Value->OnValueCommitted.AddDynamic(this, &ThisClass::OnValueCommitted);
	Value->OnValueChanged.AddDynamic(this, &ThisClass::OnValueChanged);
}

void UTCUITableProperty::NativeDestruct()
{
	/*Since Value is BindWidget, it must be valid or the Blueprint will not compile*/
	check(Value);
	Value->OnBeginSliderMovement.RemoveDynamic(this, &ThisClass::OnBeginValueMovement);
	Value->OnEndSliderMovement.RemoveDynamic(this, &ThisClass::OnEndValueMovement);
	Value->OnValueCommitted.RemoveDynamic(this, &ThisClass::OnValueCommitted);
	Value->OnValueChanged.RemoveDynamic(this, &ThisClass::OnValueChanged);
	
	Super::NativeDestruct();
}

void UTCUITableProperty::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetPropertyName(PropertyDefaultName);
}

void UTCUITableProperty::OnBeginValueMovement()
{
	OnSpinBoxBeginMovement.Broadcast(this);
}

void UTCUITableProperty::OnEndValueMovement(float InValue)
{
	OnSpinBoxEndMovement.Broadcast(this, InValue);
}

void UTCUITableProperty::OnValueCommitted(float InValue, ETextCommit::Type InCommitMethod)
{
	OnSpinBoxValueCommitted.Broadcast(this, InValue, InCommitMethod);
}

void UTCUITableProperty::OnValueChanged(float InValue)
{
	OnSpinBoxValueChanged.Broadcast(this, InValue, ETextCommit::Default);
}
