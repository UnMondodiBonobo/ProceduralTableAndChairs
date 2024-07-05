// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "TCSelectableObjectInterface.generated.h"

/**
 * Interface used as a bridge of communication between a caller and a object that implements this.
 * Used coupled with a selectable object component in this project, but this can be used anywhere
 */
UINTERFACE(BlueprintType)
class TABLEANDCHAIRS_API UTCSelectableObjectInterface : public UInterface
{
	GENERATED_BODY()
};

class TABLEANDCHAIRS_API ITCSelectableObjectInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief Set selected state and trigger associated logic
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetSelected();

	/**
	 * @brief Set Focused state and trigger associated logic
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetFocused();

	/**
	 * @brief Set Default state and trigger associated logic
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetDefault();
	
	/*Blueprint Pure not allowed on interfaces*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsSelected() const;
	/*Blueprint Pure not allowed on interfaces*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsFocused() const;
	
protected:
	bool bIsSelected = false;
	bool bIsFocused = false;
};