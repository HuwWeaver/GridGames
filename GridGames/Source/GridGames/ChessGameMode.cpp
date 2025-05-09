// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameMode.h"
#include "Kismet/KismetMathLibrary.h"

void AChessGameMode::OtherMove(AGamePiece* Piece, const FPieceMovementProperties& Move)
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
					TArray<AGamePiece*> MovedPieces;
					MovedPieces.Add(Piece);
					TArray<FVector> TargetCoordinates;
					TargetCoordinates.Add(TargetCoordinate);
					TArray<AGamePiece*> CapturedPieces;
					CapturedPieces.Add(OccupyingPiece);

					ValidMoveDestinations.Add(TargetCoordinate);
					ValidMoveOutcomes.Add(TargetCoordinate, FMoveOutcome(MovedPieces, TargetCoordinates, CapturedPieces));
					return;
				}			
			}
			else
			{
				//Target Tile is empty -> En Passant possible?
				//Get Adjacent Coordinate
				FVector AdjacentCoordinate = Piece->GetCurrentCoordinate() + FVector(MovementVector.X, 0, 0);
				if (!GridMap.Contains(AdjacentCoordinate))
				{
					//Adjacent Tile is out of bounds = Invalid Move
					return;
				}

				AGridTile& AdjacentTile = *GridMap.FindRef(AdjacentCoordinate);
				if (!AdjacentTile.GetOccupied())
				{
					//Adjacent Tile is empty -> Invalid Move
					return;
				}

				AGamePiece* OccupyingPiece = AdjacentTile.GetOccupyingPiece();
				if (OccupyingPiece == nullptr)
				{
					//TODO: Remove LogTemp log, implement more robust method
					UE_LOG(LogTemp, Fatal, TEXT("OccupyingPiece is nullptr"));
					return;
				}

				if (OccupyingPiece->GetSetupProperties().bWhite == Piece->GetSetupProperties().bWhite)
				{
					//Tile Occupied by Same Team - Invalid Move
					return;
				}

				if (OccupyingPiece->GetPieceName() != "Pawn")
				{
					//OccupyingPiece not a Pawn - Invalid Move
					return;
				}

				if (OccupyingPiece != LastMovedPiece)
				{
					//OccupyingPiece not last moved piece - Invalid Move
					return;
				}

				//Check if Opposing Pawn moved two spaces
				float VectorLength = OccupyingPiece->GetCurrentCoordinate().Y - OccupyingPiece->GetPastCoordinates().Last().Y;
				VectorLength = UKismetMathLibrary::Abs(VectorLength);
				if (VectorLength >= 2)
				{
					//En Passant possible
					TArray<AGamePiece*> MovedPieces;
					MovedPieces.Add(Piece);
					TArray<FVector> TargetCoordinates;
					TargetCoordinates.Add(TargetCoordinate);
					TArray<AGamePiece*> CapturedPieces;
					CapturedPieces.Add(OccupyingPiece);

					ValidMoveDestinations.Add(TargetCoordinate);
					ValidMoveOutcomes.Add(TargetCoordinate, FMoveOutcome(MovedPieces, TargetCoordinates, CapturedPieces));
					return;
				}
				else
				{
					//En Passant not possible
					return;
				}			
			}
		}
	}
	else if (Piece->GetPieceName() == "King")
	{

	}
}
