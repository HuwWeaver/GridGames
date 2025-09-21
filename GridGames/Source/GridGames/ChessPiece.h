// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePiece.h"
#include "ChessPiece.generated.h"

UCLASS()
class GRIDGAMES_API AChessPiece : public AGamePiece
{
	GENERATED_BODY()

private:
	virtual bool CanPromote() override;

	virtual void Init(const FPieceSetupProperties& SetupData, const FPieceMovementData& MoveData) override;

protected:
	UPROPERTY(EditAnywhere)
	bool bIsWhite{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* WhiteMaterial{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* BlackMaterial{ nullptr };
};
