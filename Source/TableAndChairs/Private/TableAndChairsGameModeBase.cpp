#include "TableAndChairsGameModeBase.h"
#include "TCCameraController.h"
#include <EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <Kismet/GameplayStatics.h>

#include "Actors/TCHUD.h"
#include "Actors/TCTable.h"
#include "Utilities/TCGeometrySpawner.h"


ATableAndChairsGameModeBase::ATableAndChairsGameModeBase()
{
	PlayerControllerClass = ATCCameraController::StaticClass();
}

void ATableAndChairsGameModeBase::OnMousePressed()
{
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController)
	{
		return;
	}

	/*Project the screen mouse location + its direction to extract a possible hit actor in that direction*/
	FVector MousePosition{};
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
	FVector MouseWorldLocation{};
	FVector MouseWorldDirection{};
	PlayerController->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, MouseWorldLocation,
		MouseWorldDirection);
	FVector HitPosition = MouseWorldLocation + (MouseWorldDirection * BIG_NUMBER);
	FHitResult HitResult {};
	if(UKismetSystemLibrary::LineTraceSingle(this, MouseWorldLocation, HitPosition, ETraceTypeQuery::TraceTypeQuery4,
		false, {PlayerController->GetPawn()}, EDrawDebugTrace::None, HitResult, true))
	{
		Internal_OnHit(HitResult);
	}
	
}

void ATableAndChairsGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if(!GetWorld())
	{
		return;
	}
	
	if(!InputMappingContext)
	{
		#if WITH_EDITOR
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
				FString::Printf(
					TEXT("Input Mapping COntext not selected in %s, select a valid Mapping Context"),
					*GetName()));
		}
		#endif
		return;
	}
	
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if(!PlayerController)
	{
		return;
	}

	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if(!LocalPlayer)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if(!InputSubsystem)
	{
		return;
	}

	/*Add the selected mapping context to the subsystem and create the geometry spawner that handles geometry generation*/
	InputSubsystem->AddMappingContext(InputMappingContext, 0);
	GeometrySpawner = GetWorld()->SpawnActor<ATCGeometrySpawner>();
}

void ATableAndChairsGameModeBase::Internal_OnHit(const FHitResult& InHitResult)
{
	if(HittableFloorName.IsNone())
	{
		#if WITH_EDITOR
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
				FString::Printf(
					TEXT("Floor Tag Name not inserted in %s, insert a valid Name"),
					*GetName()));
		}
		#endif
		return;
	}

	if(!GeometryToSpawnOnHit)
	{
		#if WITH_EDITOR
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
				FString::Printf(
					TEXT("Invalid geometry class set in %s, insert a valid geometry class"),
					*GetName()));
		}
		#endif
		return;
	}
	
	AActor* HitActor = InHitResult.GetActor();
	if(!HitActor)
	{
		return;
	}

	/*If the hit actor has a tag matching the floor tag*/
	if(HitActor->Tags.Contains(HittableFloorName))
	{
		if(InHitResult.ImpactNormal.Equals(FVector{0.f, 0.f, 1.f}, 0.1))
		{
			/*The floor was hit, spawn the selected geometry*/
			GeometrySpawner->TrySpawnGeometry(GeometryToSpawnOnHit, InHitResult.Location);
		}
	}
	/*it's possible that another table has been hit*/
	else
	{
		if(ATCTable* TableHit = Cast<ATCTable>(HitActor))
		{
			const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
			check(PlayerController);
			AHUD* HUD = PlayerController->GetHUD();
			if(!HUD)
			{
				return;
			}

			/*Cast the current HUD to a TCHUD which has all the custom functionalities needed*/
			if(ATCHUD* TCHUD = Cast<ATCHUD>(HUD))
			{
				TCHUD->OnTableHit(TableHit);
			}
		}
	}
}
