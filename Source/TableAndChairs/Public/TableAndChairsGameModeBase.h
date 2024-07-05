#pragma once

#include <CoreMinimal.h>
#include <GameFramework/GameModeBase.h>
#include <EnhancedInput/Public/InputMappingContext.h>
#include "TableAndChairsGameModeBase.generated.h"

class ATCLeggedSurfaceGeometry;
class ATCGeometrySpawner;

UCLASS()
class TABLEANDCHAIRS_API ATableAndChairsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATableAndChairsGameModeBase();

	UFUNCTION(BlueprintCallable)
	void OnMousePressed();

protected:
	virtual void BeginPlay() override;
	void Internal_OnHit(const FHitResult& InHitResult);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY(EditAnywhere)
	FName HittableFloorName;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATCLeggedSurfaceGeometry> GeometryToSpawnOnHit;
	UPROPERTY()
	TObjectPtr<ATCGeometrySpawner> GeometrySpawner;
	
};
