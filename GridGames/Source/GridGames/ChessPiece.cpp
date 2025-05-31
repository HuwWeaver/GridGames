// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPiece.h"

bool AChessPiece::CanPromote()
{
	UE_LOG(LogTemp, Display, TEXT("Checking Promotion..."));

	if (PieceName != "Pawn")
	{
		UE_LOG(LogTemp, Display, TEXT("Piece not a Pawn, no promotion"));
		return false;
	}

	if (SetupProperties.StartingCoordinates.Y - CurrentCoordinate.Y == 6 ||
		SetupProperties.StartingCoordinates.Y - CurrentCoordinate.Y == -6)
	{
		UE_LOG(LogTemp, Display, TEXT("Piece is a pawn and has reached promotion row"));
		return true;
	}

	return false;
}

void AChessPiece::PromotePiece()
{
	UE_LOG(LogTemp, Display, TEXT("%s has been promoted!"), *GetName());
}
