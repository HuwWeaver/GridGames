// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridGameGlobals.generated.h"

class AGamePiece;

UENUM(BlueprintType)
enum class ETurnPhase : uint8
{
	PreTurn UMETA(DisplayName = "PreTurn"),
	MainTurn UMETA(DisplayName = "MainTurn"),
	PostTurn UMETA(DisplayName = "PostTurn")
};


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
	UMaterialInstance* Material{ nullptr };
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

USTRUCT(BlueprintType)
struct GRIDGAMES_API FMoveOutcome
{
	GENERATED_BODY()

	TArray<AGamePiece*> MovedPieces;
	TArray<FVector> TargetCoordinates;
	TArray<AGamePiece*> CapturedPieces;
};

UCLASS()
class GRIDGAMES_API UGridGameGlobals : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};
