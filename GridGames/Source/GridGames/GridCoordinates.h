// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GridCoordinates.generated.h"

/** 
 * 
 */
USTRUCT(BlueprintType)
struct GRIDGAMES_API FGridCoordinates
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y{0};
};
