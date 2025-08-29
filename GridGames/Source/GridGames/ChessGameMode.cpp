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
			//Target Tile is out of bounds = Invalid Move
			if (!GridMap.Contains(TargetCoordinate))
			{

				return;
			}

			AGridTile& TargetTile = *GridMap.FindRef(TargetCoordinate);
			//Target Tile Occupied -> Standard Capture possible?
			if (TargetTile.GetOccupied())
			{
				AGamePiece* OccupyingPiece = TargetTile.GetOccupyingPiece();
				if (OccupyingPiece == nullptr)
				{
					//TODO: Remove LogTemp log, implement more robust method
					UE_LOG(LogTemp, Fatal, TEXT("OccupyingPiece is nullptr"));
					return;
				}

				//Tile Occupied by Opposing Team = Standard Capture, Valid Move
				if (OccupyingPiece->GetSetupProperties().bWhite != Piece->GetSetupProperties().bWhite)
				{					
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
			//Target Tile is empty -> En Passant possible?
			else
			{
				FVector AdjacentCoordinate = Piece->GetCurrentCoordinate() + FVector(MovementVector.X, 0, 0);
				//Adjacent Tile is out of bounds = Invalid Move
				if (!GridMap.Contains(AdjacentCoordinate))
				{
					
					return;
				}

				AGridTile& AdjacentTile = *GridMap.FindRef(AdjacentCoordinate);
				//Adjacent Tile is empty -> Invalid Move
				if (!AdjacentTile.GetOccupied())
				{
					
					return;
				}

				AGamePiece* OccupyingPiece = AdjacentTile.GetOccupyingPiece();
				if (OccupyingPiece == nullptr)
				{
					//TODO: Remove LogTemp log, implement more robust method
					UE_LOG(LogTemp, Fatal, TEXT("OccupyingPiece is nullptr"));
					return;
				}

				//Tile Occupied by Same Team - Invalid Move
				if (OccupyingPiece->GetSetupProperties().bWhite == Piece->GetSetupProperties().bWhite)
				{
					
					return;
				}

				//OccupyingPiece not a Pawn - Invalid Move
				if (OccupyingPiece->GetPieceName() != "Pawn")
				{
					
					return;
				}

				//OccupyingPiece not last moved piece - Invalid Move
				if (OccupyingPiece != LastMovedPiece)
				{
					
					return;
				}

				//Check if Opposing Pawn moved two spaces
				float VectorLength = OccupyingPiece->GetCurrentCoordinate().Y - OccupyingPiece->GetPastCoordinates().Last().Y;
				VectorLength = UKismetMathLibrary::Abs(VectorLength);

				//En Passant possible
				if (VectorLength >= 2)
				{
					
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
				//En Passant not possible
				else 
				{
					
					return;
				}			
			}
		}
	}
	else if (Piece->GetPieceName() == "King")
	{
		if (Move.MoveName == "Castle")
		{
			if (Piece->GetNumMovesMade() < 1)
			{
				FVector NormalizedMovementVector = Move.MovementVector;
				NormalizedMovementVector.Normalize();
				if (!Piece->GetSetupProperties().bWhite) NormalizedMovementVector *= -1;

				for (int i = 1; i <= GridColumns; i++)
				{
					FVector TileCoordinate = Piece->GetCurrentCoordinate() + (NormalizedMovementVector * i);
					//Target Tile is out of bounds = Invalid Move
					if (!GridMap.Contains(TileCoordinate))
					{
						return;
					}

					AGridTile& TargetTile = *GridMap.FindRef(TileCoordinate);

					//Tile not occupied, Castling still possible -> continue to next tile
					if (!TargetTile.GetOccupied())
					{
						continue;
					}

					AGamePiece* OccupyingPiece = TargetTile.GetOccupyingPiece();

					//Tile Occupied by Opposing Team = Invalid Move
					if (OccupyingPiece->GetSetupProperties().bWhite != Piece->GetSetupProperties().bWhite)
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
						if (!Piece->GetSetupProperties().bWhite) KingMovementVector *= -1;
						FVector KingTargetCoordinate = Piece->GetCurrentCoordinate() + KingMovementVector;
						MovedPieces.Add(Piece);
						TargetCoordinates.Add(KingTargetCoordinate);

						FVector RookTargetCoordinate = Piece->GetCurrentCoordinate() + NormalizedMovementVector;
						MovedPieces.Add(OccupyingPiece);
						TargetCoordinates.Add(RookTargetCoordinate);

						ValidMoveDestinations.Add(KingTargetCoordinate);
						ValidMoveOutcomes.Add(KingTargetCoordinate, FMoveOutcome(MovedPieces, TargetCoordinates));
					}
				}
			}
		}
	}
}

void AChessGameMode::OnTriggerPromotion(AGamePiece* Piece)
{
	ProvidePromotionChoice(Piece);
}

void AChessGameMode::PromotePiece(AGamePiece* Piece, const FName& NewPieceName)
{
	if (Piece == nullptr)
	{
		//TODO: Remove LogTemp log, implement more robust method
		UE_LOG(LogTemp, Error, TEXT("PromotePiece called with nullptr Piece"));
		return;
	}
	if (NewPieceName == "")
	{
		//TODO: Remove LogTemp log, implement more robust method
		UE_LOG(LogTemp, Error, TEXT("PromotePiece called with empty NewPieceName"));
		return;
	}

	FPieceMovementData* NewMoveData = PiecesMovementData->FindRow<FPieceMovementData>(NewPieceName, "");

	Piece->Promote(NewPieceName, *NewMoveData);
	GameTracker.LogPromotion(NewPieceName);

	PostTurn();
}
