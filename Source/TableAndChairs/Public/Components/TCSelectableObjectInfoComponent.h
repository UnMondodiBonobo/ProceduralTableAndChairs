// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include "TCSelectableObjectInfoComponent.generated.h"

/*Struct used to have a set of useful values for the color, emissive color and their respective names in the chosen material*/
USTRUCT(BlueprintType)
struct FTCSelectableObjectInfoData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor Color {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EmissiveColorValue = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ColorParamName {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName EmissiveValueParamName {};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class TABLEANDCHAIRS_API UTCSelectableObjectInfoComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTCSelectableObjectInfoComponent();

	UFUNCTION(BlueprintPure)
	const FTCSelectableObjectInfoData& GetSelectedInfo() const;
	UFUNCTION(BlueprintPure)
	const FTCSelectableObjectInfoData& GetFocusedInfo() const;
	UFUNCTION(BlueprintPure)
	const FTCSelectableObjectInfoData& GetDefaultInfo() const;
	
protected:
	UPROPERTY(EditAnywhere)
	FTCSelectableObjectInfoData SelectedInfo {};
	UPROPERTY(EditAnywhere)
	FTCSelectableObjectInfoData FocusedInfo {};
	UPROPERTY(EditAnywhere)
	FTCSelectableObjectInfoData DefaultInfo {};
};

inline const FTCSelectableObjectInfoData& UTCSelectableObjectInfoComponent::GetSelectedInfo() const
{
	return SelectedInfo;
}

inline const FTCSelectableObjectInfoData& UTCSelectableObjectInfoComponent::GetFocusedInfo() const
{
	return FocusedInfo;
}

inline const FTCSelectableObjectInfoData& UTCSelectableObjectInfoComponent::GetDefaultInfo() const
{
	return DefaultInfo;
}