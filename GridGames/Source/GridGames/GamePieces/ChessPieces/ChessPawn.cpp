// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPawn.h"
#include "GridGames/GameBoard/GameBoard.h"
#include "Kismet/KismetMathLibrary.h"

void AChessPawn::CheckPromotion()
{
	Super::CheckPromotion();

	if (SetupProperties.StartingCoordinates.Y - CurrentCoordinate.Y == 6 ||
		SetupProperties.StartingCoordinates.Y - CurrentCoordinate.Y == -6)
	{
		//Can promote, provide options to player
		UE_LOG(LogTemp, Display, TEXT("Providing Promotion Choice..."));
		ProvidePromotionChoice(this);
	}
}

void AChessPawn::OtherMove(const FPieceMovementProperties& Move)
{
	if (Move.MoveName == "FirstMove")
	{
		if (GetNumMovesMade() < 1)
		{
			FVector MovementVector = Move.MovementVector;
			if (!GetSetupProperties().bWhite) MovementVector *= -1;

			//Normalize to get intermediary tile
			MovementVector.Normalize();

			FVector IntemediaryCoordinate = GetCurrentCoordinate() + MovementVector;

			AGridTile& IntemediaryTile = *GameBoard->GetGridMap().FindRef(IntemediaryCoordinate);
			if (!IntemediaryTile.GetOccupied())
			{
				//Intemediary Tile Free -> Continue to check Target tile
				StepMove(Move);
				return;
			}
		}
	}
	else if (Move.MoveName == "Capture")
	{
		FVector MovementVector = Move.MovementVector;
		if (!GetSetupProperties().bWhite) MovementVector *= -1;

		FVector TargetCoordinate = GetCurrentCoordinate() + MovementVector;
		//Target Tile is out of bounds = Invalid Move
		if (!GameBoard->GetGridMap().Contains(TargetCoordinate))
		{

			return;
		}

		AGridTile& TargetTile = *GameBoard->GetGridMap().FindRef(TargetCoordinate);
		//Target Tile Occupied -> Standard Capture possible?
		if (TargetTile.GetOccupied())
		{
			AGamePiece* OccupyingPiece = TargetTile.GetOccupyingPiece();
			if (OccupyingPiece == nullptr)
			{
				UE_LOG(LogGridGameError, Error, TEXT("OccupyingPiece is nullptr"));
				return;
			}

			//Tile Occupied by Opposing Team = Standard Capture, Valid Move
			if (OccupyingPiece->GetSetupProperties().bWhite != GetSetupProperties().bWhite)
			{
				TArray<AGamePiece*> MovedPieces;
				MovedPieces.Add(this);
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
			FVector AdjacentCoordinate = GetCurrentCoordinate() + FVector(MovementVector.X, 0, 0);
			//Adjacent Tile is out of bounds = Invalid Move
			if (!GameBoard->GetGridMap().Contains(AdjacentCoordinate))
			{

				return;
			}

			AGridTile& AdjacentTile = *GameBoard->GetGridMap().FindRef(AdjacentCoordinate);
			//Adjacent Tile is empty -> Invalid Move
			if (!AdjacentTile.GetOccupied())
			{

				return;
			}

			AGamePiece* OccupyingPiece = AdjacentTile.GetOccupyingPiece();
			if (OccupyingPiece == nullptr)
			{
				UE_LOG(LogGridGameError, Error, TEXT("OccupyingPiece is nullptr"));
				return;
			}

			//Tile Occupied by Same Team - Invalid Move
			if (OccupyingPiece->GetSetupProperties().bWhite == GetSetupProperties().bWhite)
			{

				return;
			}

			//OccupyingPiece not a Pawn - Invalid Move
			if (OccupyingPiece->GetPieceName() != "Pawn")
			{

				return;
			}

			//OccupyingPiece not last moved piece - Invalid Move
			if (OccupyingPiece != GameBoard->GetLastMovedPiece())
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
				MovedPieces.Add(this);
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
