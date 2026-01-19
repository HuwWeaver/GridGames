// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPiece.h"

void AChessPiece::Init(const bool white, const FVector startingCoords, const AGameBoard* InGameBoard)
{
	Super::Init(white, startingCoords, InGameBoard);
	bIsWhite = white;

	if (bIsWhite && WhiteMaterial)
	{
		PieceMesh->SetMaterial(0, WhiteMaterial);
	}
	else if (!bIsWhite && BlackMaterial)
	{
		PieceMesh->SetMaterial(0, BlackMaterial);
	}
}
