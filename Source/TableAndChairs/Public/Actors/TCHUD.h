// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/HUD.h>
#include "ViewModels/TCTableViewModel.h"
#include "TCHUD.generated.h"

class UTCUIMainPage;

UCLASS()
class TABLEANDCHAIRS_API ATCHUD : public AHUD
{
	GENERATED_BODY()

public:
	/**
	 * @brief Functionalities to trigger once a table is hit
	 * @param InTable Hit table
	 */
	UFUNCTION(BlueprintCallable)
	void OnTableHit(ATCTable* InTable);

	void OnRenderFailed();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTCUIMainPage> MainPageClass;

private:
	/*Function used to clean up destroyed key Table inside the TableViewModels TMap since we don't want an invalid
	 * weak pointer as key
	 */
	UFUNCTION()
	void Internal_ClearInvalidKey(AActor* InDestroyedActor);
	UPROPERTY()
	TMap<TWeakObjectPtr<ATCTable>, TObjectPtr<UTCTableViewModel>> TableViewModels;
	UPROPERTY()
	TObjectPtr<UTCUIMainPage> MainPageWidget;
};
