// Fill out your copyright notice in the Description page of Project Settings.

#include "GridGameGameMode.h"
#include "GridGameGlobals.h"
#include "GridGames/GamePieces/GamePiece.h"
#include "GridGames/GameBoard/GridTile.h"

#include "Kismet/KismetStringLibrary.h"
#include "Kismet/BlueprintMapLibrary.h"
#include "Kismet/GameplayStatics.h"

// Functions to set up and start the game, such as Begin Play, CreateGrid, and PopulateBoard.
#pragma region Game Start
// Called when the game starts or when spawned
void AGridGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	GameBoard = Cast<AGameBoard>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameBoard::StaticClass()));
	GameBoard->BoardPopulated.AddDynamic(this, &AGridGameGameMode::GameStart);

	GameBoard->CreateGrid(GridRows, GridColumns, GridLayers, PiecesSetupData);
}

void AGridGameGameMode::GameStart()
{
	PreTurn();
}
#pragma endregion

#pragma region PreTurn
void AGridGameGameMode::PreTurn()
{
	CurrentTurnPhase = ETurnPhase::PreTurn;
	TurnStart.Broadcast();
	//TODO: Change Player/Camera
	//TODO: Determine if in Check
	//TODO: Find & Highlight Valid Pieces

	MainTurn();
}
#pragma endregion

// Functions to handle the main actions of a turn, such as Piece Selection, Valid Moved Calculation, Movement, and Piece Deselection.
#pragma region Main Turn
void AGridGameGameMode::MainTurn()
{
	CurrentTurnPhase = ETurnPhase::MainTurn;
	//TODO: Enable Input

	//Now wait for player input...
}

// This function is called when a game piece is selected.
// It retrieves the movement data for the selected piece and calculates all valid move destinations based on the piece's movement properties.
// It then shows the valid move tiles on the grid by calling ShowValidMove(true) on each valid tile.
void AGridGameGameMode::PieceSelected(AGamePiece* Piece)
{
	FPieceMovementData MovementData = Piece->GetMovementData();
	ValidMoveDestinations.Empty();

	for (FPieceMovementProperties& Move : MovementData.FullMoveList)
	{
		switch (Move.MoveType)
		{
		default:
			break;

		case EMovementTypes::Step:
			StepMove(Piece, Move);
			break;

		case EMovementTypes::LimitedRange:
			RangeMove(Piece, Move, Move.RangeLimit);
			break;

		case EMovementTypes::BoundlessRange:
			RangeMove(Piece, Move);
			break;

		case EMovementTypes::Other:
			OtherMove(Piece, Move);
			break;
		}
	}

	for (const FVector& TileCoordinate : ValidMoveDestinations)
	{
		if (GameBoard->GetGridMap().Contains(TileCoordinate))
		{
			GameBoard->GetGridMap().FindRef(TileCoordinate)->ShowValidMove(true);
		}
	}
}

// This function is called when a game piece is deselected.
// It hides all valid move tiles for that game piece by calling ShowValidMove(false) on each valid tile and clears the list of valid move destinations.
void AGridGameGameMode::PieceDeselected()
{
	for (const FVector& TileCoordinate : ValidMoveDestinations)
	{
		if (GameBoard->GetGridMap().Contains(TileCoordinate))
		{
			GameBoard->GetGridMap().FindRef(TileCoordinate)->ShowValidMove(false);
		}
	}

	ValidMoveDestinations.Empty();
}

// Step Moves are moves with a single target tile
void AGridGameGameMode::StepMove(AGamePiece* Piece, const FPieceMovementProperties& Move)
{	
	// Get the movement vector and adjust it based on the piece's team color (white or black).
	FVector MovementVector = Move.MovementVector;
	if (!Piece->GetSetupProperties().bWhite) MovementVector *= -1;

	// Get the target coordinate by adding the movement vector to the piece's current coordinate.
	FVector TargetCoordinate = Piece->GetCurrentCoordinate() + MovementVector;

	// Check if the target coordinate is within the bounds of the grid.
	if (!GameBoard->GetGridMap().Contains(TargetCoordinate))
	{
		//Target Tile is out of bounds = Invalid Move
		return;
	}

	// Get the target tile from the grid map using the target coordinate.
	AGridTile& TargetTile = *GameBoard->GetGridMap().FindRef(TargetCoordinate);

	// Check if the target tile is occupied by another piece.
	if (!TargetTile.GetOccupied())
	{
		//Empty Tile = Valid Move
		TArray<AGamePiece*> MovedPieces;
		MovedPieces.Add(Piece);
		TArray<FVector> TargetCoordinates;
		TargetCoordinates.Add(TargetCoordinate);

		// Add the target coordinate to the list of valid move destinations and outcomes.
		ValidMoveDestinations.Add(TargetCoordinate);
		ValidMoveOutcomes.Add(TargetCoordinate, FMoveOutcome(MovedPieces, TargetCoordinates));

		return;
	}

	// If the target tile is occupied, get the piece occupying it.
	AGamePiece* OccupyingPiece = TargetTile.GetOccupyingPiece();
	if (OccupyingPiece == nullptr)
	{
		UE_LOG(LogGridGameError, Error, TEXT("OccupyingPiece is nullptr"));
		return;
	}

	// Check if the occupying piece is on the same team as the moving piece.
	if (OccupyingPiece->GetSetupProperties().bWhite == Piece->GetSetupProperties().bWhite)
	{
		//Tile Occupied by Same Team = Invalid Move
		return;
	}

	// Check if the move can capture an opposing piece.
	if (Move.bCanCapture)
	{
		//Tile occupied by opposing team && Move can capture = Valid Move
		TArray<AGamePiece*> MovedPieces;
		MovedPieces.Add(Piece);
		TArray<FVector> TargetCoordinates;
		TargetCoordinates.Add(TargetCoordinate);
		TArray<AGamePiece*> CapturedPieces;
		CapturedPieces.Add(OccupyingPiece);

		// Add the target coordinate to the list of valid move destinations and outcomes.
		ValidMoveDestinations.Add(TargetCoordinate);
		ValidMoveOutcomes.Add(TargetCoordinate, FMoveOutcome(MovedPieces, TargetCoordinates, CapturedPieces));
		return;
	}

	//Move cannot capture = Invalid Move
	return;
}

// Range Moves are moves with a movement vector that can be applied multiple times, up to a range limit, to reach multiple target tiles.
void AGridGameGameMode::RangeMove(AGamePiece* Piece, const FPieceMovementProperties& Move, const int& RangeLimit)
{
	// Get the movement vector and adjust it based on the piece's team color (white or black).
	FVector MovementVector = Move.MovementVector;
	if (!Piece->GetSetupProperties().bWhite) MovementVector *= -1;

	// Get the target coordinate by adding the movement vector to the piece's current coordinate, then reduce the remaining range.
	FVector TargetCoordinate = Piece->GetCurrentCoordinate() + MovementVector;
	int RemainingRange = RangeLimit - 1;

	// Check if the target coordinate is within the bounds of the grid.
	while (GameBoard->GetGridMap().Contains(TargetCoordinate))
	{
		// Get the target tile from the grid map using the target coordinate.
		AGridTile& TargetTile = *GameBoard->GetGridMap().FindRef(TargetCoordinate);

		// Check if the target tile is occupied by another piece.
		if (!TargetTile.GetOccupied())
		{
			//Empty Tile = Valid Move
			TArray<AGamePiece*> MovedPieces;
			MovedPieces.Add(Piece);
			TArray<FVector> TargetCoordinates;
			TargetCoordinates.Add(TargetCoordinate);

			// Add the target coordinate to the list of valid move destinations and outcomes.
			ValidMoveDestinations.Add(TargetCoordinate);
			ValidMoveOutcomes.Add(TargetCoordinate, FMoveOutcome(MovedPieces, TargetCoordinates));

			// Check if the range limit is -99 (indicating no range limit).
			if (RangeLimit == -99)
			{
				//Move to Next Tile
				TargetCoordinate = TargetCoordinate + MovementVector;
				continue;
			}

			// Check if the remaining range is less than or equal to 0, in which case the range limit has been reached.
			if (RemainingRange <= 0)
			{
				//Range Limit Reached
				return;
			}
			else
			{
				//Range Remaining - Move to Next Tile
				TargetCoordinate = TargetCoordinate + MovementVector;
				RemainingRange--;
				continue;
			}
		}

		// If the target tile is occupied, check the piece occupying it.
		AGamePiece* OccupyingPiece = TargetTile.GetOccupyingPiece();
		if (OccupyingPiece == nullptr)
		{
			UE_LOG(LogGridGameError, Error, TEXT("OccupyingPiece is nullptr"));
			return;
		}

		// Check if the occupying piece is on the same team as the moving piece.
		if (OccupyingPiece->GetSetupProperties().bWhite == Piece->GetSetupProperties().bWhite)
		{
			//Tile Occupied by Same Team = Invalid Move
			return;
		}

		// Check if the move can capture an opposing piece.
		if (Move.bCanCapture)
		{
			//Tile occupied by opposing team && Move can capture = Valid Move
			TArray<AGamePiece*> MovedPieces;
			MovedPieces.Add(Piece);
			TArray<FVector> TargetCoordinates;
			TargetCoordinates.Add(TargetCoordinate);
			TArray<AGamePiece*> CapturedPieces;
			CapturedPieces.Add(OccupyingPiece);

			// Add the target coordinate to the list of valid move destinations and outcomes.
			ValidMoveDestinations.Add(TargetCoordinate);
			ValidMoveOutcomes.Add(TargetCoordinate, FMoveOutcome(MovedPieces, TargetCoordinates, CapturedPieces));
			return;
		}

		//Move cannot capture = Invalid Move
		return;
	}
}

// This function attempts to move a game piece to a target tile, checking if the move is valid based on the valid move destinations and outcomes.
// If the target tile is not in the valid move destinations or outcomes, it logs an error and returns without moving the piece.
// If the move is valid, it retrieves the move outcome and moves the piece(s) accordingly, capturing any pieces that are part of the outcome, before deselecting the piece and logging the completed move.
void AGridGameGameMode::TryMovePiece(AGamePiece* Piece, AGridTile* TargetTile)
{
	if (!ValidMoveDestinations.Contains(TargetTile->GetCoordinates()))
	{
		UE_LOG(LogGridGameError, Error, TEXT("Target Coordinate not contained in Valid Move Destinations"));
		return;
	}
	
	if (!ValidMoveOutcomes.Contains(TargetTile->GetCoordinates()))
	{
		UE_LOG(LogGridGameError, Error, TEXT("Target Coordinate not contained in Valid Move Outcomes"));
		return;
	}

	FMoveOutcome MoveOutcome = ValidMoveOutcomes.FindRef(TargetTile->GetCoordinates());

	for (int i = 0; i < MoveOutcome.MovedPieces.Num(); i++)
	{
		AGamePiece* PieceToMove = MoveOutcome.MovedPieces[i];
		FVector TargetCoordinate = MoveOutcome.TargetCoordinates[i];
		PieceToMove->Move(GameBoard->GetGridMap().FindRef(TargetCoordinate), 200);
	}

	for (AGamePiece* PieceToCapture : MoveOutcome.CapturedPieces)
	{
		PieceToCapture->PieceCaptured();
	}

	LastMovedPiece = Piece;
	PieceMoved.Broadcast();
	GameTracker.LogCompletedMove(MoveOutcome);

	PieceDeselected();

	PostTurn();
}
#pragma endregion

#pragma region PostTurn
void AGridGameGameMode::GoToPostTurn()
{
	PostTurn();
}

void AGridGameGameMode::PostTurn()
{
	CurrentTurnPhase = ETurnPhase::PostTurn;

	//TODO: Disable Input

	PreTurn();
}
#pragma endregion