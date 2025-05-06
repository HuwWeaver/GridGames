// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameMode.h"

void AChessGameMode::OtherMove(const AGamePiece* Piece, const FPieceMovementProperties& Move)
{
	if (Piece->GetPieceName() == "Pawn")
	{
		if (Move.MoveName == "FirstMove")
		{
			if (Piece->GetNumMovesMade() < 1)
			{
				FVector MovementVector = Move.MovementVector;
				if (!Piece->GetSetupProperties().bWhite) MovementVector *= -1;

				//Normalize to get intermediary tile
				MovementVector.Normalize();

				FVector IntemediaryCoordinate = Piece->GetCurrentCoordinate() + MovementVector;

				AGridTile& IntemediaryTile = *GridMap.FindRef(IntemediaryCoordinate);
				if (!IntemediaryTile.GetOccupied())
				{
					//Intemediary Tile Free -> Continue to check Target tile
					StepMove(Piece, Move);
					return;
				}		
			}
		}
		else if (Move.MoveName == "Capture")
		{
			FVector MovementVector = Move.MovementVector;
			if (!Piece->GetSetupProperties().bWhite) MovementVector *= -1;

			FVector TargetCoordinate = Piece->GetCurrentCoordinate() + MovementVector;
			if (!GridMap.Contains(TargetCoordinate))
			{
				//Target Tile is out of bounds = Invalid Move
				return;
			}

			AGridTile& TargetTile = *GridMap.FindRef(TargetCoordinate);
			if (TargetTile.GetOccupied())
			{
				AGamePiece* OccupyingPiece = TargetTile.GetOccupyingPiece();
				if (OccupyingPiece == nullptr)
				{
					//TODO: Remove LogTemp log, implement more robust method
					UE_LOG(LogTemp, Fatal, TEXT("OccupyingPiece is nullptr"));
					return;
				}

				if (OccupyingPiece->GetSetupProperties().bWhite != Piece->GetSetupProperties().bWhite)
				{
					//Tile Occupied by Opposing Team = Standard Capture, Valid Move
					ValidTiles.Add(&TargetTile);
					return;
				}			
			}
			else
			{
				//Target Tile is empty -> En Passant possible?				
			}

		}
	}
	else if (Piece->GetPieceName() == "King")
	{

	}
}
