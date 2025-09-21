// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPiece.h"

bool AChessPiece::CanPromote()
{
	if (PieceName != "Pawn")
	{
		return false;
	}

	if (SetupProperties.StartingCoordinates.Y - CurrentCoordinate.Y == 6 ||
		SetupProperties.StartingCoordinates.Y - CurrentCoordinate.Y == -6)
	{
		return true;
	}

	return false;
}

void AChessPiece::Init(const FPieceSetupProperties& SetupData, const FPieceMovementData& MoveData)
{
	Super::Init(SetupData, MoveData);
	bIsWhite = SetupData.bWhite;

	if (bIsWhite && WhiteMaterial)
	{
		PieceMesh->SetMaterial(0, WhiteMaterial);
	}
	else if (!bIsWhite && BlackMaterial)
	{
		PieceMesh->SetMaterial(0, BlackMaterial);
	}
}
