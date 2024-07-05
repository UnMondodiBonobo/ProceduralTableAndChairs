// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TCHUD.h"

#include "Actors/TCTable.h"
#include <Blueprint/UserWidget.h>
#include "ViewModels/TCTableViewModel.h"
#include "Widgets/TCUIMainPage.h"

void ATCHUD::OnTableHit(ATCTable* InTable)
{
	if(!InTable)
	{
		return;
	}

	if(!InTable->GetClass()->ImplementsInterface(UTCSelectableObjectInterface::StaticClass()))
	{
		return;
	}

	/*Check if the table is focused or selected and if is contained in the map already*/
	const bool bIsTableSelected = ITCSelectableObjectInterface::Execute_IsSelected(InTable);
	const bool bIsTableFocused = ITCSelectableObjectInterface::Execute_IsFocused(InTable);
	if(TableViewModels.Contains(InTable) && (bIsTableFocused || bIsTableSelected))
	{
		/*If it's contained in the map we need to remove the binding on its OnDestroyed
		 * invocation list since we bound when it first came in, the we need to set its state to default
		 * and remove it from the map. At last, notify the main page of the changes and let it do
		 * its own logic
		 */
		InTable->OnDestroyed.RemoveDynamic(this, &ThisClass::Internal_ClearInvalidKey);
		const TObjectPtr<UTCTableViewModel>* TableViewModel = TableViewModels.Find(InTable);
		check(TableViewModel);
		MainPageWidget->RemoveViewModel(*TableViewModel);
		TableViewModels.Remove(InTable);
		return;
	}

	/*If the table is not contained in the map, we need to create a new view model,
	 *bind to its destroyed event to clean up the map in case this specific weakptr key
	 *will be null, set the state of the table to focused, add it to the map with its corresponding
	 *view model and notify the main page of the changes and let it do its own logic
	 */
	UTCTableViewModel* NewTableViewModel = NewObject<UTCTableViewModel>(this);
	check(NewTableViewModel);
	NewTableViewModel->Initialize(InTable);
	TableViewModels.Emplace(InTable, NewTableViewModel);
	InTable->OnDestroyed.AddDynamic(this, &ThisClass::Internal_ClearInvalidKey);
	MainPageWidget->AddViewModel(NewTableViewModel);
}

void ATCHUD::OnRenderFailed()
{
	if(!MainPageWidget)
	{
		return;
	}
	
	MainPageWidget->NotifyOnRenderFailed();
}

void ATCHUD::BeginPlay()
{
	Super::BeginPlay();

	if(!MainPageClass)
	{
		#if WITH_EDITOR
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
				FString::Printf(
					TEXT("Main Page class not selected in %s, insert a valid Main Page class"),
					*GetName()));
		}
			#endif
		return;
	}

	check(GetWorld());
	MainPageWidget = CreateWidget<UTCUIMainPage>(GetWorld(), MainPageClass);
	check(MainPageWidget);
	MainPageWidget->AddToPlayerScreen();
}

void ATCHUD::Internal_ClearInvalidKey(AActor* InDestroyedActor)
{
	if(!InDestroyedActor->GetClass()->ImplementsInterface(UTCSelectableObjectInterface::StaticClass()))
	{
		return;
	}
	
	if(ATCTable* TableDestroyed = Cast<ATCTable>(InDestroyedActor))
	{
		/*If this destroyed table is contained in the map, unbind from it, remove it from the map
		 *since it will be null in a short time
		 * and set its state to default (not mandatory but ok)
		 */
		if(TableViewModels.Contains(Cast<ATCTable>(InDestroyedActor)))
		{
			TableDestroyed->OnDestroyed.RemoveDynamic(this, &ThisClass::Internal_ClearInvalidKey);
			ITCSelectableObjectInterface::Execute_SetDefault(InDestroyedActor);
			TableViewModels.Remove(TableDestroyed);
		}
	}
}
