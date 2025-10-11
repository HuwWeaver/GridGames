// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPiece.h"

void AChessPiece::Init(const FPieceSetupProperties& SetupData, AGameBoard* InGameBoard)
{
	Super::Init(SetupData, InGameBoard);
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
