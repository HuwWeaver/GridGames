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
	virtual void Init(const FPieceSetupProperties& SetupData, AGameBoard* InGameBoard) override;

protected:
	UPROPERTY(EditAnywhere)
	bool bIsWhite{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* WhiteMaterial{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* BlackMaterial{ nullptr };
};
