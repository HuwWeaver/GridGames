// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridGames/GamePieces/ChessPiece.h"
#include "ChessPawn.generated.h"

UCLASS()
class GRIDGAMES_API AChessPawn : public AChessPiece
{
	GENERATED_BODY()

private:
	virtual void CheckPromotion() override;
};
