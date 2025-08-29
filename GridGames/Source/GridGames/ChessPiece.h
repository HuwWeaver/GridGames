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
};
