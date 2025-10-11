// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessKing.h"
#include "GridGames/GameBoard/GameBoard.h"

void AChessKing::OtherMove(const FPieceMovementProperties& Move)
{
	if (Move.MoveName == "Castle")
	{
		if (GetNumMovesMade() < 1)
		{
			FVector NormalizedMovementVector = Move.MovementVector;
			NormalizedMovementVector.Normalize();
			if (!GetSetupProperties().bWhite) NormalizedMovementVector *= -1;

			for (int i = 1; i <= GameBoard->GetGridColumns(); i++)
			{
				FVector TileCoordinate = GetCurrentCoordinate() + (NormalizedMovementVector * i);
				//Target Tile is out of bounds = Invalid Move
				if (!GameBoard->GetGridMap().Contains(TileCoordinate))
				{
					return;
				}

				AGridTile& TargetTile = *GameBoard->GetGridMap().FindRef(TileCoordinate);

				//Tile not occupied, Castling still possible -> continue to next tile
				if (!TargetTile.GetOccupied())
				{
					continue;
				}

				AGamePiece* OccupyingPiece = TargetTile.GetOccupyingPiece();

				//Tile Occupied by Opposing Team = Invalid Move
				if (OccupyingPiece->GetSetupProperties().bWhite != GetSetupProperties().bWhite)
				{

					return;
				}

				//OccupyingPiece not a Rook = Invalid Move
				if (OccupyingPiece->GetPieceName() != "Rook")
				{
					return;
				}

				//Rook not moved either, Castling possible
				if (OccupyingPiece->GetNumMovesMade() < 1)
				{
					TArray<AGamePiece*> MovedPieces;
					TArray<FVector> TargetCoordinates;

					FVector KingMovementVector = Move.MovementVector;
					if (!GetSetupProperties().bWhite) KingMovementVector *= -1;
					FVector KingTargetCoordinate = GetCurrentCoordinate() + KingMovementVector;
					MovedPieces.Add(this);
					TargetCoordinates.Add(KingTargetCoordinate);

					FVector RookTargetCoordinate = GetCurrentCoordinate() + NormalizedMovementVector;
					MovedPieces.Add(OccupyingPiece);
					TargetCoordinates.Add(RookTargetCoordinate);

					ValidMoveDestinations.Add(KingTargetCoordinate);
					ValidMoveOutcomes.Add(KingTargetCoordinate, FMoveOutcome(MovedPieces, TargetCoordinates));
				}
			}
		}
	}
}