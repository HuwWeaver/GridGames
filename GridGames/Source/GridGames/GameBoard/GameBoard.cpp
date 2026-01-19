// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoard/GameBoard.h"
#include "GridGameGlobals.h"
#include "GridGames/GamePieces/GamePiece.h"

// Sets default values
AGameBoard::AGameBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameBoard::BeginPlay()
{
	Super::BeginPlay();
}

// Creates the grid of tiles using specified AGridTile class and specified grid dimensions.
void AGameBoard::CreateGrid(int rows, int columns, int layers, UDataTable* PieceData)
{
	GridRows = rows;
	GridColumns = columns;
	GridLayers = layers;

	for (size_t Column = 0; Column < columns; Column++) {
		for (size_t Row = 0; Row < rows; Row++) {
			for (size_t Layer = 0; Layer < layers; Layer++) {

				FVector Location = FVector(Row * 200, Column * 200, Layer * 500.0f);
				FRotator Rotation(0, 0, 0);
				FActorSpawnParameters SpawnInfo;

				AGridTile* Tile = GetWorld()->SpawnActor<AGridTile>(GridTileClass, Location, Rotation, SpawnInfo);
				Tile->Init(FVector(Column, Row, Layer));
				GridMap.Add(FVector(Column, Row, Layer), Tile);
			}
		}
	}

	PopulateBoard(PieceData);
}

// Populates the grid using the specified SetupData and MovementData DataTables with the specified GamePiece class.
// The SetupData DataTable contains the initial setup properties for each piece, such as starting coordinates and team color.
void AGameBoard::PopulateBoard(UDataTable* PieceData)
{
	if (!PieceData) UE_LOG(LogGridGameFatal, Fatal, TEXT("No Valid Setup Data"));

	TArray<FName> SetupDataRows = PieceData->GetRowNames();

	for (FName RowName : SetupDataRows)
	{
		FPieceSetupProperties* Row = PieceData->FindRow<FPieceSetupProperties>(RowName, "");

		// X and Y are swapped for first vector as grid X axis doesn't follow world X axis, but rather follows world Y (and vice versa).
		FVector Location = FVector(Row->StartingCoordinates.Y * 200, Row->StartingCoordinates.X * 200, 0.0) + FVector(200 / 2, 200 / 2, 100);
		FRotator Rotation = Row->bWhite ? FRotator(0, 0, 0) : FRotator(0, 180, 0);
		FActorSpawnParameters SpawnInfo;

		AGamePiece* Piece = GetWorld()->SpawnActor<AGamePiece>(Row->PieceClass, Location, Rotation, SpawnInfo);

		Piece->Init(Row->bWhite, Row->StartingCoordinates, this);
	}

	BoardPopulated.Broadcast();
}

void AGameBoard::PieceMoved(AGamePiece* Piece)
{
	LastMovedPiece = Piece;

	for (auto& Tile : GridMap)
	{
		Tile.Value->ShowValidMove(false);
	}
}

// Called every frame
void AGameBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

