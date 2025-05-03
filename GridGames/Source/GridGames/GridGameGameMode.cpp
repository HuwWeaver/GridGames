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
	UE_LOG(LogTemp, Display, TEXT("Creating Grid..."));

	for (size_t Column = 0; Column < GridColumns; Column++){
		for (size_t Row = 0; Row < GridRows; Row++) {

			FVector Location = FVector(Row * TileSize, Column * TileSize, 0.0f);
			FRotator Rotation(0,0,0);
			FActorSpawnParameters SpawnInfo;

			AGridTile* Tile = GetWorld()->SpawnActor<AGridTile>(GridTileClass, Location, Rotation, SpawnInfo);
			Tile->Init(FVector(Column, Row, 0));
			GridMap.Add(FVector(Column, Row, 0), Tile);
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Grid Created!"));
	PopulateBoard();
}

void AGridGameGameMode::PopulateBoard()
{
	UE_LOG(LogTemp, Display, TEXT("Populating Board..."));

	TArray<FName> SetupDataRows = SetupData->GetRowNames();

	for (FName RowName : SetupDataRows)
	{
		FPieceSetupProperties* Row = SetupData->FindRow<FPieceSetupProperties>(RowName, "");

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

		FText PieceName = FText::FromString(StringName);
		FPieceMovementData* MoveData = PiecesData->FindRow<FPieceMovementData>(UKismetStringLibrary::Conv_StringToName(StringName), "");

		Piece->Init(PieceName, *Row, *MoveData);
	}

	UE_LOG(LogTemp, Display, TEXT("Board Populated!"));
}

void AGridGameGameMode::StepMove(TArray<AGridTile*>& ValidTiles, const AGamePiece* Piece, const FPieceMovementProperties& Move)
{
	FVector TargetCoordinate = Piece->GetCurrentCoordinate() + Move.MovementVector;
	AGridTile& TargetTile = *GridMap.FindRef(TargetCoordinate);
	if (!TargetTile.GetOccupied())
	{
		//Empty Tile = Valid Move
		ValidTiles.Add(&TargetTile);
		return;
	}

	AGamePiece* OccupyingPiece = TargetTile.GetOccupyingPiece();
	if (OccupyingPiece == nullptr)
	{
		UE_LOG(LogTemp, Fatal, TEXT("OccupyingPiece is nullptr"));
		return;
	}

	if (OccupyingPiece->SetupProperties.bWhite == Piece->SetupProperties.bWhite)
	{
		//Tile Occupied by Same Team = Invalid Move
		return;
	}

	if (Move.bCanCapture)
	{
		//Tile occupied by opposing team &&
		//Move can capture = Valid Move
		ValidTiles.Add(&TargetTile);
		return;
	}

	//Move cannot capture = Invalid Move
	return;
}

void AGridGameGameMode::RangeMove(TArray<AGridTile*>& ValidTiles, const AGamePiece* Piece, const FPieceMovementProperties& Move, const int& RangeLimit)
{
	FVector TargetCoordinate = Piece->GetCurrentCoordinate() + Move.MovementVector;
	int RangeLeft = RangeLimit - 1;

	while (GridMap.Contains(TargetCoordinate))
	{
		AGridTile& TargetTile = *GridMap.FindRef(TargetCoordinate);

		if (!TargetTile.GetOccupied())
		{
			//Empty Tile = Valid Move
			ValidTiles.Add(&TargetTile);

			if (RangeLimit == -99)
			{
				//No Range Limit Specified - Boundless Range
				//Move to Next Tile
				TargetCoordinate = TargetCoordinate + Move.MovementVector;
				continue;
			}

			if (RangeLeft <= 0)
			{
				//Range Limit Reached
				return;
			}
			else
			{
				//Range Remaining - Move to Next Tile
				TargetCoordinate = TargetCoordinate + Move.MovementVector;
				RangeLeft--;
				continue;
			}
		}

		AGamePiece* OccupyingPiece = TargetTile.GetOccupyingPiece();
		if (OccupyingPiece == nullptr)
		{
			UE_LOG(LogTemp, Fatal, TEXT("OccupyingPiece is nullptr"));
			return;
		}

		if (OccupyingPiece->GetSetupProperties().bWhite == Piece->SetupProperties.bWhite)
		{
			//Tile Occupied by Same Team = Invalid Move
			return;
		}

		if (Move.bCanCapture)
		{
			//Tile occupied by opposing team &&
			//Move can capture = Valid Move
			ValidTiles.Add(&TargetTile);
			return;
		}

		//Move cannot capture = Invalid Move
		return;
	}
}

void AGridGameGameMode::OtherMove(TArray<AGridTile*>& ValidTiles, const AGamePiece* Piece, const FPieceMovementProperties& Move)
{
}

void AGridGameGameMode::OnPieceSelected(AGamePiece* Piece)
{
	FPieceMovementData& MovementData = Piece->MovementData;
	TArray<AGridTile*> ValidTiles;

	for (FPieceMovementProperties& Move : MovementData.FullMoveList)
	{
		switch (Move.MoveType)
		{
		default:
			break;

		case EMovementTypes::Step:
			StepMove(ValidTiles, Piece, Move);
			break;

		case EMovementTypes::LimitedRange:
			RangeMove(ValidTiles, Piece, Move, Move.RangeLimit);
			break;

		case EMovementTypes::BoundlessRange:
			RangeMove(ValidTiles, Piece, Move);
			break;

		case EMovementTypes::Other:
			OtherMove(ValidTiles, Piece, Move);
			break;
		}
	}

	for (AGridTile* Tile : ValidTiles)
	{
		Tile->ShowValidMove(true);
	}
}
