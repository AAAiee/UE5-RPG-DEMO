// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

// This class does not need to be modified. For reflection system only
UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLASH_API IHitInterface
{
	GENERATED_BODY()

public:

	// can be implemented in both blueprint and C++, don't need virtual anymore, UE5 handles this for us
	UFUNCTION(BlueprintNativeEvent)
	void GetHit(const FVector& ImpactPoint);
};
