// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TCChairPoolComponent.h"

#include "Actors/TCChair.h"

UTCChairPoolComponent::UTCChairPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

ATCChair* UTCChairPoolComponent::GetChairResource()
{
	if (!ChairType)
	{
		return nullptr;
	}

	/*If the pool is empty, try to generate some resources*/
	if (ResourcesPool.IsEmpty())
	{
		if (!Internal_IncreaseReusablePool(PoolIncreaseSize))
		{
			return nullptr;
		}
	}

	/*return the chair extracted from the pool*/
	ATCChair* ReusableChair = ResourcesPool.Pop(false);
	ReusableChair->RegisterAllComponents();
	return ReusableChair;
}

bool UTCChairPoolComponent::AddChairResource(ATCChair* InChairResource)
{
	if (!ChairType)
	{
		return false;
	}

	if (!InChairResource)
	{
		return false;
	}

	/*Returns the chair to the pool and unregister all components*/
	InChairResource->ClearGeometryData();
	ResourcesPool.Emplace(InChairResource);
	return true;
}

void UTCChairPoolComponent::BeginPlay()
{
	Super::BeginPlay();
	Internal_IncreaseReusablePool(StartingPoolSize);
}

bool UTCChairPoolComponent::Internal_IncreaseReusablePool(int16 InIncreasePoolSize)
{
	if(!GetOwner())
	{
		return false;
	}
	
	if (!ChairType)
	{
		return false;
	}

	if (InIncreasePoolSize <= 0)
	{
		return false;
	}

	/*Increase the pool resources and add some new ones*/
	ResourcesPool.Reserve(ResourcesPool.Max() + InIncreasePoolSize);
	for (int32 i = 0; i < InIncreasePoolSize; ++i)
	{
		ATCChair* NewChair = GetWorld()->SpawnActor<ATCChair>(ChairType);
		ResourcesPool.Emplace(NewChair);
	}

	return true;
}

