// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridGameData.generated.h"


USTRUCT(BlueprintType)
struct GRIDGAMES_API FPieceSetupProperties : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWhite{ false };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartingCoordinates{ 0,0,0 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* PieceMesh{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* Material;
};

UENUM(BlueprintType)
enum class EMovementTypes : uint8
{
	Step UMETA(DisplayName = "Step"),
	LimitedRange UMETA(DisplayName = "LimitedRange"),
	BoundlessRange UMETA(DisplayName = "BoundlessRange"),
	Other UMETA(DisplayName = "Other")
};

USTRUCT(BlueprintType)
struct GRIDGAMES_API FPieceMovementProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MovementVector{ 0,0,0 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMovementTypes MoveType{ EMovementTypes::Step };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanCapture{ true };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RangeLimit{ 0 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MoveName{ "" };
};

USTRUCT(BlueprintType)
struct GRIDGAMES_API FPieceMovementData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPieceMovementProperties> FullMoveList;
};

UCLASS()
class GRIDGAMES_API UGridGameData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
