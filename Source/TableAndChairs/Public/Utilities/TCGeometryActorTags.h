// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Engine/DeveloperSettings.h>

#include <GameplayTagContainer.h>
#include "TCGeometryActorTags.generated.h"

/**
 * Developer settings used to easily access geometry actors tags association to use
 * in any point of the code
 */
UCLASS(Config = TCGeometryActorTags, DefaultConfig)
class TABLEANDCHAIRS_API UTCGeometryActorTags : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Config)
	FGameplayTag TableTagRoot {};
	UPROPERTY(EditAnywhere, Config)
	FGameplayTag ChairTagRoot {};
	UPROPERTY(EditAnywhere, Config)
	FGameplayTag TableWidthTag {};
	UPROPERTY(EditAnywhere, Config)
	FGameplayTag TableLengthTag {};
	UPROPERTY(EditAnywhere, Config)
	FGameplayTag TableHeightTag {};
	UPROPERTY(EditAnywhere, Config)
	FGameplayTag TableLegsHeightTag {};
	UPROPERTY(EditAnywhere, Config)
	FGameplayTag ChairsWidthTag {};
	UPROPERTY(EditAnywhere, Config)
	FGameplayTag ChairsLengthTag {};
	UPROPERTY(EditAnywhere, Config)
	FGameplayTag ChairsHeightTag {};
	UPROPERTY(EditAnywhere, Config)
	FGameplayTag ChairsLegsHeightTag {};
	UPROPERTY(EditAnywhere, Config)
	FGameplayTag ChairsBackHeightTag {};
	UPROPERTY(EditAnywhere, Config)
	FGameplayTag ChairsSpacingTag {};
};
