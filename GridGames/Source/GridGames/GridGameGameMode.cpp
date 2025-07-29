// Fill out your copyright notice in the Description page of Project Settings.

#include "GridGameGameMode.h"
#include "GridGameData.h"

#include "Kismet/KismetStringLibrary.h"
#include "Kismet/BlueprintMapLibrary.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void AGridGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	CreateGrid();
}

// Creates the grid of tiles using specified AGridTile class and specified grid dimensions.
void AGridGameGameMode::CreateGrid()
{
	//TODO: Remove LogTemp log, implement more robust method
	UE_LOG(LogTemp, Display, TEXT("Creating Grid..."));

	for (size_t Column = 0; Column < GridColumns; Column++){
		for (size_t Row = 0; Row < GridRows; Row++) {
			for (size_t Layer = 0; Layer < GridLayers; Layer++) {

				FVector Location = FVector(Row * TileSize, Column * TileSize, Layer * 500.0f);
				FRotator Rotation(0, 0, 0);
				FActorSpawnParameters SpawnInfo;

				AGridTile* Tile = GetWorld()->SpawnActor<AGridTile>(GridTileClass, Location, Rotation, SpawnInfo);
				Tile->Init(FVector(Column, Row, Layer));
				GridMap.Add(FVector(Column, Row, Layer), Tile);
			}
		}
	}

	//TODO: Remove LogTemp log, implement more robust method
	UE_LOG(LogTemp, Display, TEXT("Grid Created!"));
	PopulateBoard();
}

// Populates the grid using the specified SetupData and MovementData DataTables with the specified GamePiece class.
// The SetupData DataTable contains the initial setup properties for each piece, such as starting coordinates and team color.
// The MovementData DataTable contains the movement properties for each piece, such as movement vectors and range limits.
void AGridGameGameMode::PopulateBoard()
{
	//TODO: Remove LogTemp log, implement more robust method
	UE_LOG(LogTemp, Display, TEXT("Populating Board..."));

	//TODO: Remove LogTemp log, implement more robust method
	if (!PiecesSetupData || !PiecesMovementData) UE_LOG(LogTemp, Fatal, TEXT("No Valid Setup or Movement Data"));

	TArray<FName> SetupDataRows = PiecesSetupData->GetRowNames();

	for (FName RowName : SetupDataRows)
	{
		FPieceSetupProperties* Row = PiecesSetupData->FindRow<FPieceSetupProperties>(RowName, "");

		// X and Y are swapped for first vector as grid X axis doesn't follow world X axis, but rather follows world Y (and vice versa).
		FVector Location = FVector(Row->StartingCoordinates.Y * TileSize, Row->StartingCoordinates.X * TileSize, 0.0) + FVector(TileSize / 2, TileSize / 2, 100);
		FRotator Rotation = Row->bWhite ? FRotator(0,0,0) : FRotator(0,180,0);
		FActorSpawnParameters SpawnInfo;

		AGamePiece* Piece = GetWorld()->SpawnActor<AGamePiece>(GamePieceClass, Location, Rotation, SpawnInfo);

		FString StringName = RowName.ToString();
		StringName = StringName.RightChop(5);
		if (StringName.Right(1).IsNumeric())
		{
			StringName = StringName.LeftChop(1);
		}

		FName PieceName = UKismetStringLibrary::Conv_StringToName(StringName);
		FPieceMovementData* MoveData = PiecesMovementData->FindRow<FPieceMovementData>(PieceName, "");

		Piece->Init(PieceName, *Row, *MoveData);
	}

	//TODO: Remove LogTemp log, implement more robust method
	UE_LOG(LogTemp, Display, TEXT("Board Populated!"));
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
	if (!GridMap.Contains(TargetCoordinate))
	{
		//Target Tile is out of bounds = Invalid Move
		return;
	}

	// Get the target tile from the grid map using the target coordinate.
	AGridTile& TargetTile = *GridMap.FindRef(TargetCoordinate);

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
		//TODO: Remove LogTemp log, implement more robust method
		UE_LOG(LogTemp, Fatal, TEXT("OccupyingPiece is nullptr"));
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
	while (GridMap.Contains(TargetCoordinate))
	{
		// Get the target tile from the grid map using the target coordinate.
		AGridTile& TargetTile = *GridMap.FindRef(TargetCoordinate);

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
			//TODO: Remove LogTemp log, implement more robust method
			UE_LOG(LogTemp, Fatal, TEXT("OccupyingPiece is nullptr"));
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

// This function is a placeholder for other types of moves that may be implemented in derived classes - usually for special moves like castling or en passant in chess.
void AGridGameGameMode::OtherMove(AGamePiece* Piece, const FPieceMovementProperties& Move)
{
}

// This function attempts to move a game piece to a target tile, checking if the move is valid based on the valid move destinations and outcomes.
// If the target tile is not in the valid move destinations or outcomes, it logs an error and returns without moving the piece.
// If the move is valid, it retrieves the move outcome and moves the piece(s) accordingly, capturing any pieces that are part of the outcome, before deselecting the piece and logging the completed move.
void AGridGameGameMode::TryMovePiece(AGamePiece* Piece, AGridTile* TargetTile)
{
	if (!ValidMoveDestinations.Contains(TargetTile->GetCoordinates()))
	{
		//TODO: Remove LogTemp log, implement more robust method
		UE_LOG(LogTemp, Error, TEXT("Target Coordinate not contained in Valid Move Destinations"));
		return;
	}
	
	if (!ValidMoveOutcomes.Contains(TargetTile->GetCoordinates()))
	{
		//TODO: Remove LogTemp log, implement more robust method
		UE_LOG(LogTemp, Error, TEXT("Target Coordinate not contained in Valid Move Outcomes"));
		return;
	}

	FMoveOutcome MoveOutcome = ValidMoveOutcomes.FindRef(TargetTile->GetCoordinates());

	for (int i = 0; i < MoveOutcome.MovedPieces.Num(); i++)
	{
		AGamePiece* PieceToMove = MoveOutcome.MovedPieces[i];
		FVector TargetCoordinate = MoveOutcome.TargetCoordinates[i];
		PieceToMove->Move(GridMap.FindRef(TargetCoordinate), TileSize);
	}

	for (AGamePiece* PieceToCapture : MoveOutcome.CapturedPieces)
	{
		PieceToCapture->PieceCaptured();
	}

	LastMovedPiece = Piece;
	PieceMoved.Broadcast();
	GameTracker.LogCompletedMove(MoveOutcome);

	PieceDeselected();
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
		if (GridMap.Contains(TileCoordinate))
		{
			GridMap.FindRef(TileCoordinate)->ShowValidMove(true);
		}
	}
}

// This function is called when a game piece is deselected.
// It hides all valid move tiles for that game piece by calling ShowValidMove(false) on each valid tile and clears the list of valid move destinations.
void AGridGameGameMode::PieceDeselected()
{
	for (const FVector& TileCoordinate : ValidMoveDestinations)
	{
		if (GridMap.Contains(TileCoordinate))
		{
			GridMap.FindRef(TileCoordinate)->ShowValidMove(false);
		}
	}

	ValidMoveDestinations.Empty();
}

// This function is a placeholder for derived classes to implement specific promotion logic, such as providing a choice of piece to promote to.
void AGridGameGameMode::OnTriggerPromotion(AGamePiece* Piece)
{
	UE_LOG(LogTemp, Warning, TEXT("OnTriggerPromotion called for Piece: %s"), *Piece->GetPieceName().ToString());
}
