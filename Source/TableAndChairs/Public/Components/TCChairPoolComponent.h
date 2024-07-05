// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include "TCChairPoolComponent.generated.h"

class UTCChairGeometryDataAsset;
class ATCChair;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TABLEANDCHAIRS_API UTCChairPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTCChairPoolComponent();

	virtual void BeginPlay() override;

	/**
	 * @brief Returns a Chair resource from the pool if any is available, if not creates a new resource and returns it 
	 * @return The chair resource or nullptr if the operation is not successful
	 */
	ATCChair* GetChairResource();

	/**
	 * @brief Add a chair resource to the pool (usually called if a resource is no longer needed by the user)
	 * @param InChairResource Chair resource to return to the pool
	 * @return True if the operation on insertion is successful, false otherwise
	 */
	bool AddChairResource(ATCChair* InChairResource);

	/**
	 * @brief Returns the stored Geometry data asset set in blueprint
	 * @return The chair geometry data asset or nullptr if not set in blueprint
	 */
	const UTCChairGeometryDataAsset* GetChairGeometryDataAsset() const;

protected:
	UPROPERTY(EditAnywhere, DisplayName = PoolData, meta = (ClampMin = 1))
	int16 StartingPoolSize = 25;
	UPROPERTY(EditAnywhere, DisplayName = PoolData, meta = (ClampMin = 1))
	int16 PoolIncreaseSize = 5;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATCChair> ChairType;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTCChairGeometryDataAsset> ChairGeometryDataAsset;

private:
	bool Internal_IncreaseReusablePool(int16 InIncreasePoolSize);

	UPROPERTY()
	TArray<TObjectPtr<ATCChair>> ResourcesPool;
};

inline const UTCChairGeometryDataAsset* UTCChairPoolComponent::GetChairGeometryDataAsset() const
{
	return ChairGeometryDataAsset;
}
