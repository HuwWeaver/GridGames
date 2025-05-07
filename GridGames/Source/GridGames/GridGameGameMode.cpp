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

		//X and Y are swapped for first vector as grid X axis doesn't follow world X axis, but rather follows world Y (and vice versa).
		FVector Location = FVector(Row->StartingCoordinates.Y * TileSize, Row->StartingCoordinates.X * TileSize, 0.0) + FVector(TileSize / 2, TileSize / 2, 100);
		FRotator Rotation(0, 0, 0);
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

void AGridGameGameMode::StepMove(const AGamePiece* Piece, const FPieceMovementProperties& Move)
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
	if (!TargetTile.GetOccupied())
	{
		//Empty Tile = Valid Move
		ValidMoveDestinations.Add(TargetCoordinate);
		return;
	}

	AGamePiece* OccupyingPiece = TargetTile.GetOccupyingPiece();
	if (OccupyingPiece == nullptr)
	{
		//TODO: Remove LogTemp log, implement more robust method
		UE_LOG(LogTemp, Fatal, TEXT("OccupyingPiece is nullptr"));
		return;
	}

	if (OccupyingPiece->GetSetupProperties().bWhite == Piece->GetSetupProperties().bWhite)
	{
		//Tile Occupied by Same Team = Invalid Move
		return;
	}

	if (Move.bCanCapture)
	{
		//Tile occupied by opposing team &&
		//Move can capture = Valid Move
		ValidMoveDestinations.Add(TargetCoordinate);
		return;
	}

	//Move cannot capture = Invalid Move
	return;
}

void AGridGameGameMode::RangeMove(const AGamePiece* Piece, const FPieceMovementProperties& Move, const int& RangeLimit)
{
	FVector MovementVector = Move.MovementVector;
	if (!Piece->GetSetupProperties().bWhite) MovementVector *= -1;

	FVector TargetCoordinate = Piece->GetCurrentCoordinate() + MovementVector;
	int RemainingRange = RangeLimit - 1;

	while (GridMap.Contains(TargetCoordinate))
	{
		AGridTile& TargetTile = *GridMap.FindRef(TargetCoordinate);

		if (!TargetTile.GetOccupied())
		{
			//Empty Tile = Valid Move
			ValidMoveDestinations.Add(TargetCoordinate);

			if (RangeLimit == -99)
			{
				//No Range Limit Specified - Boundless Range
				//Move to Next Tile
				TargetCoordinate = TargetCoordinate + MovementVector;
				continue;
			}

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

		AGamePiece* OccupyingPiece = TargetTile.GetOccupyingPiece();
		if (OccupyingPiece == nullptr)
		{
			//TODO: Remove LogTemp log, implement more robust method
			UE_LOG(LogTemp, Fatal, TEXT("OccupyingPiece is nullptr"));
			return;
		}

		if (OccupyingPiece->GetSetupProperties().bWhite == Piece->GetSetupProperties().bWhite)
		{
			//Tile Occupied by Same Team = Invalid Move
			return;
		}

		if (Move.bCanCapture)
		{
			//Tile occupied by opposing team &&
			//Move can capture = Valid Move
			ValidMoveDestinations.Add(TargetCoordinate);
			return;
		}

		//Move cannot capture = Invalid Move
		return;
	}
}

void AGridGameGameMode::OtherMove(const AGamePiece* Piece, const FPieceMovementProperties& Move)
{
}

void AGridGameGameMode::TryMovePiece(AGamePiece* Piece, AGridTile* TargetTile)
{
	if (ValidMoveDestinations.Contains(TargetTile->GetCoordinates()))
	{
		if (TargetTile->GetOccupied())
		{
			TargetTile->GetOccupyingPiece()->PieceCaptured();
		}

		Piece->Move(TargetTile, TileSize);
		LastMovedPiece = Piece;
		PieceMoved.Broadcast();

		PieceDeselected();
	}
	else
	{
		//TODO: Invalid Tile Selected, tell player
	}
}

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

	for (FVector TileCoordinate : ValidMoveDestinations)
	{
		if (GridMap.Contains(TileCoordinate))
		{
			GridMap.FindRef(TileCoordinate)->ShowValidMove(true);
		}
	}
}

void AGridGameGameMode::PieceDeselected()
{
	for (FVector TileCoordinate : ValidMoveDestinations)
	{
		if (GridMap.Contains(TileCoordinate))
		{
			GridMap.FindRef(TileCoordinate)->ShowValidMove(false);
		}
	}

	ValidMoveDestinations.Empty();
}
